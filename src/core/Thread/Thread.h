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

void Thread_init(Thread *thread, ThreadFunction func, void* arg);
void Thread_self(Thread *thread);
void Thread_start(Thread *thread);
void Thread_join(const Thread *thread);
void Thread_exit(void);

void Lock_init(Lock *lock);
void Lock_free(Lock *lock);
void Lock_lock(Lock *lock);
void Lock_unlock(Lock *lock);

#endif