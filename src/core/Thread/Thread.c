#include "Thread.h"

void Thread_init(Thread *const thread, ThreadFunction func, void* arg) {
    thread->arg = arg;
    thread->func = func;
}

inline void Thread_self(Thread *const thread) {
    thread->pthread = pthread_self();
}

inline void Thread_start(Thread *const thread) {
    pthread_create(&thread->pthread, NULL, thread->func, thread->arg);
}

inline void Thread_join(const Thread *const thread) {
    pthread_join(thread->pthread, NULL);
}

inline void Thread_exit(void) {
    pthread_exit(NULL);
}

inline void Lock_init(Lock *const lock) {
    lock->mutex = PTHREAD_MUTEX_INITIALIZER;
}

inline void Lock_free(Lock *const lock) {
    pthread_mutex_destroy(&lock->mutex);
}

inline void Lock_lock(Lock *const lock) {
    pthread_mutex_lock(&lock->mutex);
}

inline void Lock_unlock(Lock *const lock) {
    pthread_mutex_unlock(&lock->mutex);
}