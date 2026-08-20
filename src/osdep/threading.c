#include "osdep/threading.h"

bxc_thread_t *osdep_thread_get_current(void) {
    bxc_cs_t cs;

    /* This is based on the observation that critical sections don't touch any
     * kernel structures when there's nothing else that acquired it. It
     * initializes the critical section descriptor just enough and call
     * OSEnterCriticalSection() to grab the current thread ID. */
    cs.thr = NULL;
    cs.refcount = 0;
    OSEnterCriticalSection(&cs);
    return cs.thr;
}
