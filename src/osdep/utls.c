#include "muteki/threading.h"
#include "muteki/utils.h"
#include "osdep/heap.h"
#include "osdep/threading.h"
#include "osdep/utls.h"

typedef struct utls_container_s utls_container_t;
typedef struct utls_element_s utls_element_t;
typedef struct utls_key_s utls_key_t;
typedef struct utls_dict_s utls_dict_t;

#define UTLS_INIT_SHIFT (4u)
#define UTLS_HEADER_MAGIC (0xb0ee6f5bu)

struct utls_dict_s {
    size_t size_shift;
    size_t used;
    utls_element_t *elements;
};

struct utls_container_s {
    unsigned int magic;
    utls_dict_t dict;
    bxc_cs_t cs;
};

struct utls_key_s {
    const bxc_thread_t *desc;
    const void *stack_mem;
    bxc_thread_func_t thread_func;
};

struct utls_element_s {
    utls_key_t key;
    void *value;
};

static utls_container_t __utls;

static uint32_t murmur2(const void *key, size_t len, uint32_t seed);
static inline size_t rebuild_threshold(size_t current_shift);
static void osdep_utls_dict_init(utls_dict_t *dict, size_t desired_size_shift);
static void osdep_utls_dict_fini(utls_dict_t *dict);
static void osdep_utls_dict_grow(utls_dict_t *dict);
static utls_element_t *osdep_utls_dict_lookup(const utls_dict_t *dict, const utls_key_t *key, bool find_empty);
static void *osdep_utls_dict_get(const utls_dict_t *dict, const utls_key_t *key);
static void *osdep_utls_dict_alloc_and_set(utls_dict_t *dict, const utls_key_t *key, size_t alloc_size);

/**
 * @brief MurmurHash 2
 * @details Took from https://github.com/abrandoned/murmur2/blob/master/MurmurHash2.c
 * @param key Original key 
 * @param len 
 * @param seed 
 * @return uint32_t 
 */
static uint32_t murmur2(const void *key, size_t len, uint32_t seed) {
    /* 'm' and 'r' are mixing constants generated offline.
        They're not really 'magic', they just happen to work well.  */

    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    /* Initialize the hash to a 'random' value */

    uint32_t h = seed ^ len;

    /* Mix 4 bytes at a time into the hash */

    const unsigned char * data = (const unsigned char *) key;

    while(len >= 4)
    {
        uint32_t k = *((uint32_t*) data);

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    /* Handle the last few bytes of the input array  */

    switch(len) {
    case 3:
        h ^= data[2] << 16;
        // fall through
    case 2:
        h ^= data[1] << 8;
        // fall through
    case 1:
        h ^= data[0];
        // fall through
    default:
        h *= m;
    };

    /* Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.  */

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
} 

static inline size_t rebuild_threshold(size_t current_shift) {
    // This sets the load factor of rebuilding to 0.75.
    return ((1 << current_shift) >> 2) + ((1 << current_shift) >> 1);
}

static inline size_t dict_size(size_t shift) {
    return 1 << shift;
}

static void osdep_utls_dict_init(utls_dict_t *dict, size_t desired_size_shift) {
    if (desired_size_shift == 0) {
        desired_size_shift = UTLS_INIT_SHIFT;
    }
    size_t desired_size_nmemb = dict_size(desired_size_shift);
    dict->size_shift = desired_size_shift;
    dict->used = 0;
    dict->elements = osdep_heap_alloc(sizeof(utls_element_t) * desired_size_nmemb);
    for (size_t i = 0; i < desired_size_nmemb; i++) {
        dict->elements[i].key.desc = NULL;
        dict->elements[i].value = NULL;
    }
}

static void osdep_utls_dict_fini(utls_dict_t *dict) {
    for (size_t i = 0; i < dict_size(dict->size_shift); i++) {
        if (dict->elements[i].key.desc != NULL) {
            osdep_heap_free(dict->elements[i].value);
            dict->elements[i].value = NULL;
            dict->elements[i].key.desc = NULL;
            dict->elements[i].key.stack_mem = NULL;
            dict->elements[i].key.thread_func = NULL;
        }
    }
    osdep_heap_free(dict->elements);
    dict->elements = NULL;
    dict->size_shift = 0;
    dict->used = 0;
}

static void osdep_utls_dict_grow(utls_dict_t *dict) {
    utls_dict_t tmp;

    size_t old_size_nmemb = dict_size(dict->size_shift);

    osdep_utls_dict_init(&tmp, dict->size_shift + 1);
    for (size_t i = 0; i < old_size_nmemb; i++) {
        if (dict->elements[i].key.desc != NULL) {
            utls_element_t *element = osdep_utls_dict_lookup(dict, &dict->elements[i].key, true);
            if (element == NULL || element->key.desc != NULL) {
                WriteComDebugMsg("Failed to grow dict.");
                osdep_heap_free(tmp.elements);
                return;
            }
            element->key.desc = dict->elements[i].key.desc;
            element->key.stack_mem = dict->elements[i].key.stack_mem;
            element->key.thread_func = dict->elements[i].key.thread_func;
            element->value = dict->elements[i].value;
        }
    }
    osdep_heap_free(dict->elements);
    dict->elements = tmp.elements;
    dict->used = tmp.used;
    dict->size_shift = tmp.size_shift;
}

static utls_element_t *osdep_utls_dict_lookup(const utls_dict_t *dict, const utls_key_t *key, bool find_empty) {
    const size_t size = dict_size(dict->size_shift);
    const size_t hint_index = murmur2(key, sizeof(*key), UTLS_HEADER_MAGIC) & (size - 1);

    for (size_t i = 0; i < size; i++) {
        size_t current_index = (hint_index + i) & (size - 1);
        // Skip not allocated elements
        if (dict->elements[current_index].key.desc == NULL) {
            continue;
        }
        // Exact match
        if (
            dict->elements[current_index].key.desc == key->desc &&
            dict->elements[current_index].key.stack_mem == key->stack_mem &&
            dict->elements[current_index].key.thread_func == key->thread_func
        ) {
            return &dict->elements[current_index];
        }
    }

    if (find_empty) {
        for (size_t i = 0; i < size; i++) {
            size_t current_index = (hint_index + i) & (size - 1);
            if (dict->elements[current_index].key.desc == NULL) {
                return &dict->elements[current_index];
            }
        }
    }

    // No match after scanning the entire bucket
    return NULL;
}

// Temporarily disabling inlining here to workaround a potential instruction reordering issue.
__attribute__((noinline))
static void *osdep_utls_dict_get(const utls_dict_t *dict, const utls_key_t *key) {
    utls_element_t *e = osdep_utls_dict_lookup(dict, key, false);
    if (e == NULL) {
        return NULL;
    }
    return e->value;
}

static void *osdep_utls_dict_alloc_and_set(utls_dict_t *dict, const utls_key_t *key, size_t alloc_size) {
    if (alloc_size == 0) {
        return NULL;
    }

    if (rebuild_threshold(dict->size_shift) <= dict->used) {
        osdep_utls_dict_grow(dict);
    }

    utls_element_t *element = osdep_utls_dict_lookup(dict, key, true);
    if (element == NULL) {
        WriteComDebugMsg("osdep_utls_dict_alloc_and_set: Failed to find empty slot.");
        return NULL;
    }
    if (element->key.desc == NULL) {
        void *buf = osdep_heap_alloc(alloc_size);
        if (buf == NULL) {
            WriteComDebugMsg("osdep_utls_dict_alloc_and_set: Failed to allocate memory.");
            return NULL;
        }
        element->key.desc = key->desc;
        element->key.stack_mem = key->stack_mem;
        element->key.thread_func = key->thread_func;
        element->value = buf;
        dict->used++;
    }
    return element->value;
}

void osdep_utls_cinit(void) {
    if (__utls.magic != UTLS_HEADER_MAGIC) {
        OSInitCriticalSection(&__utls.cs);
        OSEnterCriticalSection(&__utls.cs);
        __utls.magic = UTLS_HEADER_MAGIC;
        osdep_utls_dict_init(&__utls.dict, UTLS_INIT_SHIFT);
        OSLeaveCriticalSection(&__utls.cs);
    }
}

void osdep_utls_cfini(void) {
    if (__utls.magic != UTLS_HEADER_MAGIC) {
        return;
    }
    OSEnterCriticalSection(&__utls.cs);
    __utls.magic = 0;
    osdep_utls_dict_fini(&__utls.dict);
    OSLeaveCriticalSection(&__utls.cs);
    OSDeleteCriticalSection(&__utls.cs);
}

void *osdep_utls_peek(const bxc_thread_t *thr) {
    if (thr == NULL) {
        return NULL;
    }

    OSEnterCriticalSection(&__utls.cs);

    utls_key_t key = { thr, thr->stack, thr->thread_func };
    void *val = osdep_utls_dict_get(&__utls.dict, &key);

    OSLeaveCriticalSection(&__utls.cs);

    return val;
}

void osdep_utls_get_stats(osdep_utls_stats_t *stats) {
    OSEnterCriticalSection(&__utls.cs);

    stats->is_initialized = __utls.magic == UTLS_HEADER_MAGIC;
    stats->slots_used = __utls.dict.used;
    stats->slots_allocated = dict_size(__utls.dict.size_shift);

    OSLeaveCriticalSection(&__utls.cs);
}

extern uint8_t __tdata_start;
extern uint8_t __tdata_end;
extern uint8_t __tbss_start;
extern uint8_t __tbss_end;

__attribute__((used))
static void *osdep_utls_read_tp(void) {
    osdep_utls_cinit();

    OSEnterCriticalSection(&__utls.cs);

    bxc_thread_t *thr = osdep_thread_get_current();
    utls_key_t key = { thr, thr->stack, thr->thread_func };

    void *val = osdep_utls_dict_get(&__utls.dict, &key);
    if (val == NULL) {
        size_t tdata_size = &__tdata_end - &__tdata_start;
        size_t tbss_size = &__tbss_end - &__tbss_start;

        val = osdep_utls_dict_alloc_and_set(&__utls.dict, &key, tdata_size + tbss_size + 8);
        if (val == NULL) {
            OSLeaveCriticalSection(&__utls.cs);
            WriteComDebugMsg("osdep_utls_read_tp: Cannot allocate memory. Will likely crash soon...");
            return NULL;
        }

        uint8_t *tdata_base = ((uint8_t *) val) + 8;
        uint8_t *tbss_base = tdata_base + tdata_size;

        ((uint32_t *) val)[0] = 0;
        ((uint32_t *) val)[1] = 0;
        if (tdata_size != 0) {
            for (size_t i = 0; i < tdata_size; i++) {
                tdata_base[i] = (&__tdata_start)[i];
            }
        }
        if (tbss_size != 0) {
            for (size_t i = 0; i < tbss_size; i++) {
                tbss_base[i] = 0;
            }
        }
    }

    OSLeaveCriticalSection(&__utls.cs);

    return val;
}

__attribute__((naked))
void __aeabi_read_tp(void) {
    // Save registers that are normally scratch registers except r0 to satisfy the no clobber requirements
    asm (
        "push {r1, r2, r3, r4, ip, lr}\n\t"
        "bl osdep_utls_read_tp\n\t"
        "pop {r1, r2, r3, r4, ip, lr}\n\t"
        "bx lr"
    );
}
