#include "osdep/threading.h"
#include "osdep/ktls.h"
#include "osdep/heap.h"

int osdep_ktls_init(bxc_thread_t *thr) {
    for (size_t i = 0; i < sizeof(thr->ktls) / sizeof(thr->ktls[0]); i++) {
        thr->ktls[i] = 0;
    }
    return 0;
}

void **osdep_ktls_get(bxc_thread_t *thr, unsigned int key) {
    if (key <= OSDEP_KTLS_KEY_MAX) {
        return (void **) &thr->ktls[key];
    }
    return NULL;
}

void *osdep_ktls_getvalue(bxc_thread_t *thr, unsigned int key) {
    if (key <= OSDEP_KTLS_KEY_MAX) {
        return (void *) thr->ktls[key];
    }
    return NULL;
}

int osdep_ktls_set(bxc_thread_t *thr, unsigned int key, void *value) {
    if (key <= OSDEP_KTLS_KEY_MAX) {
        thr->ktls[key] = (uintptr_t) value;
        return 0;
    }
    return -1;
}

void *osdep_ktls_alloc(bxc_thread_t *thr, unsigned int key, size_t bytes) {
    void **tls_area_p = osdep_ktls_get(thr, key);
    if (tls_area_p == NULL) {
        return NULL;
    }
    void *tls_area = *tls_area_p;
    if (tls_area != NULL) {
        return NULL;
    }
    void *allocated = osdep_heap_alloc(bytes);
    if (allocated == NULL) {
        return NULL;
    }
    *tls_area_p = allocated;
    return allocated;
}

int osdep_ktls_free(bxc_thread_t *thr, unsigned int key) {
    void *tls_area = osdep_ktls_getvalue(thr, key);
    if (tls_area == NULL) {
        return -1;
    }
    osdep_heap_free(tls_area);
    osdep_ktls_set(thr, key, NULL);
    return 0;
}

int osdep_ktls_init_self(void) {
    bxc_thread_t *thr = osdep_thread_get_current();
    if (thr == NULL) {
        return -1;
    }
    return osdep_ktls_init(thr);
}

void **osdep_ktls_get_self(unsigned int key) {
    bxc_thread_t *thr = osdep_thread_get_current();
    if (thr == NULL) {
        return NULL;
    }
    return osdep_ktls_get(thr, key);
}

void *osdep_ktls_getvalue_self(unsigned int key) {
    bxc_thread_t *thr = osdep_thread_get_current();
    if (thr == NULL) {
        return NULL;
    }
    return osdep_ktls_getvalue(thr, key);
}

int osdep_ktls_set_self(unsigned int key, void *value) {
    bxc_thread_t *thr = osdep_thread_get_current();
    if (thr == NULL) {
        return -1;
    }
    return osdep_ktls_set(thr, key, value);
}

void *osdep_ktls_alloc_self(unsigned int key, size_t bytes) {
    bxc_thread_t *thr = osdep_thread_get_current();
    if (thr == NULL) {
        return NULL;
    }
    return osdep_ktls_alloc(thr, key, bytes);
}

int osdep_ktls_free_self(unsigned int key) {
    bxc_thread_t *thr = osdep_thread_get_current();
    if (thr == NULL) {
        return -1;
    }
    return osdep_ktls_free(thr, key);
}
