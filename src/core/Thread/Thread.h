#ifndef OB_THREAD_H
#define OB_THREAD_H

#include <pthread.h>

typedef void*(OB_ThreadFunction)(void *data);

struct OB_Thread {
    pthread_t pthread;
    OB_ThreadFunction *func;
    void *arg;
};

struct OB_Lock {
    pthread_mutex_t mutex;
};

void OB_Thread_init(struct OB_Thread *thread, OB_ThreadFunction func, void* arg);
void OB_Thread_self(struct OB_Thread *thread);
void OB_Thread_start(struct OB_Thread *thread);
void OB_Thread_join(const struct OB_Thread *thread);
void OB_Thread_exit(void);

void OB_Lock_init(struct OB_Lock *lock);
void OB_Lock_free(struct OB_Lock *lock);
void OB_Lock_lock(struct OB_Lock *lock);
void OB_Lock_unlock(struct OB_Lock *lock);

#endif