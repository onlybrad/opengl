#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>

typedef void*(ThreadFunction)(void *data);

typedef struct Thread {
    pthread_t pthread;
    ThreadFunction *func;
    void *arg;
} Thread;

typedef struct Lock {
    pthread_mutex_t mutex;
} Lock;

void Thread_init(Thread *const thread, ThreadFunction func, void* arg);
void Thread_self(Thread *const thread);
void Thread_start(Thread *const thread);
void Thread_join(const Thread *const thread);
void Thread_exit(void);

void Lock_init(Lock *const lock);
void Lock_free(Lock *const lock);
void Lock_lock(Lock *const lock);
void Lock_unlock(Lock *const lock);

#endif