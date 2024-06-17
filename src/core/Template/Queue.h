#include <stdlib.h>
#include <stdio.h>
#include "../Util/util.h"

#if 0
typedef const char *str;
#define T str
#endif

#ifndef T
    #error "Missing parameter T"
#endif

#define QUEUE_INITIAL_CAPACITY 16
#define QUEUE MAKE_NAME(Queue, T)
#define RESULT MAKE_NAME(Result, T)
#define QUEUE_RESIZE MAKE_NAME(QUEUE, resize)
#define QUEUE_INIT MAKE_NAME(QUEUE, init)
#define QUEUE_FREE MAKE_NAME(QUEUE, free)
#define QUEUE_ENQUEUE MAKE_NAME(QUEUE, enqueue)
#define QUEUE_DEQUEUE MAKE_NAME(QUEUE, dequeue)

#if defined QUEUE_PRINT_FORMAT && defined QUEUE_PRINT_ARGUMENTS
#define QUEUE_PRINT MAKE_NAME(QUEUE, print)
#define QUEUE_TO_STRING MAKE_NAME(QUEUE, to_string)
#endif //QUEUE_PRINT_FORMAT QUEUE_PRINT_ARGUMENTS

#ifndef QUEUE_IMPLEMENTATION

#include "Result.h"

typedef struct QUEUE {
    T *buffer;
    size_t tail;
    size_t head;
    size_t capacity;
} QUEUE;

void QUEUE_INIT(QUEUE queue[static 1]);
void QUEUE_FREE(QUEUE queue[static 1]);
void QUEUE_ENQUEUE(QUEUE queue[static 1], T value);
RESULT QUEUE_DEQUEUE(QUEUE queue[static 1]);

#endif //QUEUE_IMPLEMENTATION

#if defined QUEUE_PRINT_FORMAT && defined QUEUE_PRINT_ARGUMENTS
void QUEUE_PRINT(const QUEUE queue[static 1]);
char *QUEUE_TO_STRING(const QUEUE queue[static 1]);
#endif //QUEUE_PRINT_FORMAT QUEUE_PRINT_ARGUMENTS

#if 0
#define QUEUE_IMPLEMENTATION
#endif

#if defined QUEUE_IMPLEMENTATION 

#include <string.h>
#include <assert.h>

static void QUEUE_RESIZE(QUEUE queue[static 1], const size_t new_capacity) {
    T *buffer = realloc(queue->buffer, sizeof(T) * new_capacity);
    assert(buffer != NULL);
    queue->buffer = buffer;
    queue->capacity = new_capacity;
}

void QUEUE_INIT(QUEUE queue[static 1]) {
    queue->buffer = malloc(sizeof(T) * QUEUE_INITIAL_CAPACITY);
    assert(queue->buffer != NULL);
    queue->capacity = QUEUE_INITIAL_CAPACITY;
    queue->head = 0;
    queue->tail = 0;
}

void QUEUE_FREE(QUEUE queue[static 1]) {
    free(queue->buffer);
    memset(queue, 0, sizeof(QUEUE));
}

void QUEUE_ENQUEUE(QUEUE queue[static 1], T value) {
    if(queue->head == queue->capacity) {
        QUEUE_RESIZE(queue, queue->capacity * 2);
    }
    queue->buffer[queue->head++] = value;
}

RESULT QUEUE_DEQUEUE(QUEUE queue[static 1]) {
    if(queue->head == 0) {
        return (RESULT){.success = false};
    }
    
    T ret = queue->buffer[queue->tail++];

    if(queue->head == queue->tail) {
        queue->head = 0;
        queue->tail = 0;
    }

    return (RESULT){.success = true, .value = ret};
}

#if defined QUEUE_PRINT_FORMAT && defined QUEUE_PRINT_ARGUMENTS
void QUEUE_PRINT(const QUEUE queue[static 1]) {
    if(queue->head == 0) {
        return;
    }

    for(size_t i=queue->tail; i<queue->head - 1; i++) {
        printf(QUEUE_PRINT_FORMAT " -> ", QUEUE_PRINT_ARGUMENTS(queue->buffer[i]));
    }

    if(queue->tail < queue->head) {
        printf(QUEUE_PRINT_FORMAT, QUEUE_PRINT_ARGUMENTS(queue->buffer[queue->head - 1]));
    }

    putchar('\n');
}

char *QUEUE_TO_STRING(const QUEUE queue[static 1]) {
    size_t length = 0;
    for(size_t i=queue->tail; i<queue->head - 1; i++) {
        const int char_printed = snprintf(NULL, 0, QUEUE_PRINT_FORMAT " -> ", QUEUE_PRINT_ARGUMENTS(queue->buffer[i]));
        length += (size_t)char_printed;
    }

    if(queue->tail < queue->head) {
        const int char_printed = snprintf(NULL, 0, QUEUE_PRINT_FORMAT, QUEUE_PRINT_ARGUMENTS(queue->buffer[queue->head - 1]));
        length += (size_t)char_printed;
    }

    char *queue_str = malloc(length * sizeof(char) + 1);
    assert(queue_str != NULL);

    size_t offset = 0;
    for(size_t i=queue->tail; i<queue->head - 1; i++) {
        const int char_printed = sprintf(queue_str + offset, QUEUE_PRINT_FORMAT " -> ", QUEUE_PRINT_ARGUMENTS(queue->buffer[i]));
        offset += (size_t)char_printed;
    }

    if(queue->tail < queue->head) {
        sprintf(queue_str + offset, QUEUE_PRINT_FORMAT, QUEUE_PRINT_ARGUMENTS(queue->buffer[queue->head - 1]));
    }

    queue_str[length] = '\0'; 

    return queue_str;
}
#endif //QUEUE_PRINT_FORMAT QUEUE_PRINT_ARGUMENTS

#endif //QUEUE_IMPLEMENTATION

#undef QUEUE_INITIAL_CAPACITY
#undef T
#undef RESULT
#undef QUEUE
#undef PREFIX
#undef QUEUE_RESIZE
#undef QUEUE_INIT
#undef QUEUE_FREE
#undef QUEUE_ENQUEUE
#undef QUEUE_DEQUEUE

#if defined QUEUE_PRINT_FORMAT && defined QUEUE_PRINT_ARGUMENTS
#undef QUEUE_PRINT
#undef QUEUE_TO_STRING
#undef QUEUE_PRINT_FORMAT
#undef QUEUE_PRINT_ARGUMENTS
#endif

#ifdef QUEUE_IMPLEMENTATION
#undef QUEUE_IMPLEMENTATION
#endif