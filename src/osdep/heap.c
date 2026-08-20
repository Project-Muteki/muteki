#include "osdep/heap.h"
#include "muteki/memory.h"  // for _lfree() and lmalloc()
#include "muteki/fs/file.h"  // for _afopen() et al

typedef struct {
    size_t usable_size;
    void *raw_ptr;
} __mchx_t;

static const size_t __OVER_ALLOC_SIZE = 4 + sizeof(__mchx_t);
static const char TRACE_START[4] = {'H', 'T', 'R', 'C'};
static bxc_fs_file_t *__heap_tracer_osfh = NULL;

// So we lose as little performance as possible when heap tracer is turned off.
#define _unlikely(x) __builtin_expect(!!(x), 0)

/**
 * @brief A hack that fixes allocator alignment by adding an extra header to allocated memchunk.
 *
 * @details
 * Besta allocator is 4-bytes aligned but EABI requires 8-bytes aligned allocations. We can use compiler directives to
 * cover this up but it might introduce performance penalty. This essentially over-allocates the memory and makes sure
 * that there are at least 8 bytes available for us to store the original pointer and the allocation size so we don't
 * have to resort to using the memchunk header to determine whether we're at the original pointer or not.
 *
 * @param q Unaligned pointer given by Besta lmalloc().
 * @param size Usable size of the allocation.
 * @return Pointer that aligns to 8-bytes.
 */
static inline void *__mchx_format(void *q, size_t size) {
    uintptr_t pp = ((((uintptr_t) q) + __OVER_ALLOC_SIZE) & (~((uintptr_t) 7u)));
    __mchx_t *mchx = (__mchx_t *) (pp - sizeof(__mchx_t));
    mchx->raw_ptr = q;
    mchx->usable_size = size;
    return (void *) pp;
}

/**
 * @brief Read size value from an mchx header.
 *
 * @param p Pointer produced by __mchx_format().
 * @return Size of usable memory.
 */
static inline size_t __mchx_get_size(const void *p) {
    uintptr_t pp = (uintptr_t) p;
    __mchx_t *mchx = (__mchx_t *) (pp - sizeof(__mchx_t));
    return mchx->usable_size;
}

/**
 * @brief Read raw pointer value from an mchx header.
 *
 * @param p Pointer produced by __mchx_format().
 * @return The original unaligned pointer.
 */
static inline void *__mchx_get_raw(void *p) {
    uintptr_t pp = (uintptr_t) p;
    __mchx_t *mchx = (__mchx_t *) (pp - sizeof(__mchx_t));
    return mchx->raw_ptr;
}

static void _heaptracer_on_malloc(void *p, size_t size) {
    uintptr_t r = (uintptr_t) p;
    size_t s = size;
    if (_unlikely(__heap_tracer_osfh != NULL)) {
        _fwrite(&r, 1, sizeof(r), __heap_tracer_osfh);
        _fwrite(&s, 1, sizeof(s), __heap_tracer_osfh);
        __fflush(__heap_tracer_osfh);
    }
}

static void _heaptracer_on_free(void *p) {
    uintptr_t r = (uintptr_t) p;
    size_t s = 0;
    if (_unlikely(__heap_tracer_osfh != NULL)) {
        _fwrite(&r, 1, sizeof(r), __heap_tracer_osfh);
        _fwrite(&s, 1, sizeof(s), __heap_tracer_osfh);
        __fflush(__heap_tracer_osfh);
    }
}

__attribute__((assume_aligned(8)))
void *osdep_heap_alloc(size_t size) {
    void *q = lmalloc(size + __OVER_ALLOC_SIZE);

    _heaptracer_on_malloc(q, size + __OVER_ALLOC_SIZE);

    if (q == NULL) {
        return NULL;
    }

    return __mchx_format(q, size);
}

size_t osdep_heap_get_alloc_size(const void *ptr) {
    return __mchx_get_size(ptr);
}

void osdep_heap_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    void *q = __mchx_get_raw(ptr);

    _heaptracer_on_free(q);
    _lfree(q);
}

bool osdep_heap_trace_start(void) {
    if (__heap_tracer_osfh != NULL) {
        return false;
    }
    // ab+ doesn't seem to work
    __heap_tracer_osfh = _afopen("C:\\HEAPT.BIN", "rb+");
    if (__heap_tracer_osfh == NULL) {
        __heap_tracer_osfh = _afopen("C:\\HEAPT.BIN", "wb+");
        if (__heap_tracer_osfh == NULL) {
            return false;
        }
    }
    __fseek(__heap_tracer_osfh, 0, _SYS_SEEK_END);
    _fwrite(TRACE_START, 1, sizeof(TRACE_START), __heap_tracer_osfh);
    __fflush(__heap_tracer_osfh);
    return true;
}

bool osdep_heap_trace_stop(void) {
    if (__heap_tracer_osfh == NULL) {
        return false;
    }
    _fclose(__heap_tracer_osfh);
    __heap_tracer_osfh = NULL;
    return true;
}
