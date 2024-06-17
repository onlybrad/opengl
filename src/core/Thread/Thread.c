#include <stdlib.h>
#include "Thread.h"

void Thread_init(Thread thread[static 1], ThreadFunction func, void* arg) {
    thread->arg = arg;
    thread->func = func;
}

inline void Thread_self(Thread thread[static 1]) {
    thread->pthread = pthread_self();
}

inline void Thread_start(Thread thread[static 1]) {
    pthread_create(&thread->pthread, NULL, thread->func, thread->arg);
}

inline void Thread_join(const Thread thread[static 1]) {
    pthread_join(thread->pthread, NULL);
}

inline void Thread_exit(void) {
    pthread_exit(NULL);
}

inline void Lock_init(Lock lock[static 1]) {
    lock->mutex = PTHREAD_MUTEX_INITIALIZER;
}

inline void Lock_free(Lock lock[static 1]) {
    pthread_mutex_destroy(&lock->mutex);
}

inline void Lock_lock(Lock lock[static 1]) {
    pthread_mutex_lock(&lock->mutex);
}

inline void Lock_unlock(Lock lock[static 1]) {
    pthread_mutex_unlock(&lock->mutex);
}