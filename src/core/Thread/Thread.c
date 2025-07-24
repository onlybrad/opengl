#include <assert.h>
#include <stdlib.h>
#include "Thread.h"

void Thread_init(Thread *thread, ThreadFunction func, void* arg) {
    assert(thread != NULL);
    assert(func != NULL);

    thread->arg = arg;
    thread->func = func;
}

inline void Thread_self(Thread *thread) {
    assert(thread != NULL);

    thread->pthread = pthread_self();
}

inline void Thread_start(Thread *thread) {
    assert(thread != NULL);

    pthread_create(&thread->pthread, NULL, thread->func, thread->arg);
}

inline void Thread_join(const Thread *thread) {
    assert(thread != NULL);

    pthread_join(thread->pthread, NULL);
}

inline void Thread_exit(void) {
    pthread_exit(NULL);
}

inline void Lock_init(Lock *lock) {
    assert(lock != NULL);

    lock->mutex = PTHREAD_MUTEX_INITIALIZER;
}

inline void Lock_free(Lock *lock) {
    assert(lock != NULL);

    pthread_mutex_destroy(&lock->mutex);
}

inline void Lock_lock(Lock *lock) {
    assert(lock != NULL);

    pthread_mutex_lock(&lock->mutex);
}

inline void Lock_unlock(Lock *lock) {
    assert(lock != NULL);

    pthread_mutex_unlock(&lock->mutex);
}