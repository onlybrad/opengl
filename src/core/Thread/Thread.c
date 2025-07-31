#include <assert.h>
#include <stdlib.h>
#include "thread.h"

void OB_Thread_init(struct OB_Thread *thread, OB_ThreadFunction func, void* arg) {
    assert(thread != NULL);
    assert(func != NULL);

    thread->arg = arg;
    thread->func = func;
}

void OB_Thread_self(struct OB_Thread *thread) {
    assert(thread != NULL);

    thread->pthread = pthread_self();
}

void OB_Thread_start(struct OB_Thread *thread) {
    assert(thread != NULL);

    pthread_create(&thread->pthread, NULL, thread->func, thread->arg);
}

void OB_Thread_join(const struct OB_Thread *thread) {
    assert(thread != NULL);

    pthread_join(thread->pthread, NULL);
}

void OB_Thread_exit(void) {
    pthread_exit(NULL);
}

void OB_Lock_init(struct OB_Lock *lock) {
    assert(lock != NULL);

    lock->mutex = PTHREAD_MUTEX_INITIALIZER;
}

void OB_Lock_free(struct OB_Lock *lock) {
    assert(lock != NULL);

    pthread_mutex_destroy(&lock->mutex);
}

void OB_Lock_lock(struct OB_Lock *lock) {
    assert(lock != NULL);

    pthread_mutex_lock(&lock->mutex);
}

void OB_Lock_unlock(struct OB_Lock *lock) {
    assert(lock != NULL);

    pthread_mutex_unlock(&lock->mutex);
}