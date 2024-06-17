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

void Thread_init(Thread thread[static 1], ThreadFunction func, void* arg);
void Thread_self(Thread thread[static 1]);
void Thread_start(Thread thread[static 1]);
void Thread_join(const Thread thread[static 1]);
void Thread_exit(void);

#define SYNCHRONIZE(LOCK, CODE)\
Lock_lock(LOCK);\
CODE \
Lock_unlock(LOCK);\

void Lock_init(Lock lock[static 1]);
void Lock_free(Lock lock[static 1]);
void Lock_lock(Lock lock[static 1]);
void Lock_unlock(Lock lock[static 1]);

#endif