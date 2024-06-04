#include <stdlib.h>
#include <stdio.h>
#include "../Util/util.h"

//USAGE EXAMPLE
//pointer types and multi word types must be typedefed into a single word
//one translation unit must define VECTOR_IMPLEMENTATION to get the function implementations for a specific T type
//Vector_{T}_{Function Name}
#if 0
typedef const char *str;
#define T str
#define VECTOR_IMPLEMENTATION
//#include Vector.h
#endif

#ifndef T
    #error "Missing parameter T"
#endif

#define VECTOR_DEFAULT_CAPACITY 16
#define VECTOR MAKE_NAME(Vector, T)
#define VECTOR_RESIZE MAKE_NAME(VECTOR, resize)
#define VECTOR_INIT MAKE_NAME(VECTOR, init)
#define VECTOR_INIT_MOVE MAKE_NAME(VECTOR, init_move)
#define VECTOR_INIT_COPY MAKE_NAME(VECTOR, init_copy)
#define VECTOR_COPY MAKE_NAME(VECTOR, copy)
#define VECTOR_FREE MAKE_NAME(VECTOR, free)
#define VECTOR_PUSH MAKE_NAME(VECTOR, push)
#define VECTOR_POP MAKE_NAME(VECTOR, pop)
#define VECTOR_AT MAKE_NAME(VECTOR, at)
#define VECTOR_SET MAKE_NAME(VECTOR, set)
#define VECTOR_REMOVE MAKE_NAME(VECTOR, remove)

#if defined VECTOR_PRINT_FORMAT && defined VECTOR_PRINT_ARGUMENTS
#define VECTOR_PRINT MAKE_NAME(VECTOR, print)
#define VECTOR_TO_STRING MAKE_NAME(VECTOR, to_string)
#endif //VECTOR_PRINT_FORMAT VECTOR_PRINT_ARGUMENTS

#ifndef VECTOR_IMPLEMENTATION
typedef struct VECTOR {
    T *buffer;
    size_t length;
    size_t capacity;
} VECTOR;

void VECTOR_INIT(VECTOR *const vector, const size_t capacity);
void VECTOR_INIT_MOVE(VECTOR *const vector, T *buffer, const size_t length);
void VECTOR_INIT_COPY(VECTOR *const vector, T *buffer, const size_t length);
void VECTOR_COPY(VECTOR *vector_dst, VECTOR *const vector_src);
void VECTOR_FREE(VECTOR *const vector);
void VECTOR_PUSH(VECTOR *const vector, T value);
T VECTOR_POP(VECTOR *const vector);
T VECTOR_AT(const VECTOR *const vector, const size_t index);
void VECTOR_SET(VECTOR *const vector, const size_t index, T value);
T VECTOR_REMOVE(VECTOR *const vector, const size_t index);

#if defined VECTOR_PRINT_FORMAT && defined VECTOR_PRINT_ARGUMENTS
void VECTOR_PRINT(const VECTOR *const vector);
char *VECTOR_TO_STRING(const VECTOR *const vector);
#endif //VECTOR_PRINT_FORMAT VECTOR_PRINT_ARGUMENTS

#else

#include <string.h>
#include <assert.h>
#include <math.h>

static void VECTOR_RESIZE(VECTOR *const vector, const size_t new_capacity) {
    T *buffer = realloc(vector->buffer, sizeof(T) * new_capacity);
    assert(buffer != NULL);
    vector->buffer = buffer;
    vector->capacity = new_capacity;
}

void VECTOR_INIT(VECTOR *const vector, const size_t capacity) {
    vector->capacity = capacity == 0 ? VECTOR_DEFAULT_CAPACITY : capacity;
    vector->length = 0;
    vector->buffer = calloc(vector->capacity, sizeof(T));
    assert(vector->buffer != NULL);
}

void VECTOR_INIT_MOVE(VECTOR *const vector, T *buffer, const size_t length) {
    vector->capacity = length;
    vector->length = length;
    vector->buffer = buffer;
}

void VECTOR_INIT_COPY(VECTOR *const vector, T *buffer, const size_t length) {
    vector->capacity = length;
    vector->length = length;
    vector->buffer = malloc(sizeof(T) * length);
    assert(vector->buffer != NULL);
    memcpy(vector->buffer, buffer, sizeof(T) * length);
}

void VECTOR_FREE(VECTOR *const vector) {
    free(vector->buffer);
    memset(vector, 0, sizeof(VECTOR));
}

void VECTOR_COPY(VECTOR *vector_dst, VECTOR *const vector_src) {
    vector_dst->buffer = malloc(sizeof(T) * vector_src->capacity);
    assert(vector_dst->buffer != NULL);
    memcpy(vector_dst->buffer, vector_src->buffer, sizeof(T) * vector_src->capacity);
    vector_dst->capacity = vector_src->capacity;
    vector_dst->length = vector_src->length;
}

void VECTOR_PUSH(VECTOR *const vector, T value) {
    if(vector->length == vector->capacity) {
        VECTOR_RESIZE(vector, vector->capacity * 2);
    }
    vector->buffer[vector->length++] = value;
}

T VECTOR_POP(VECTOR *const vector) {
    assert(vector->length > 0);

    return vector->buffer[--vector->length];
}

T VECTOR_AT(const VECTOR *const vector, const size_t index) {
    assert(index < vector->length);

    return vector->buffer[index];
}

void VECTOR_SET(VECTOR *const vector, const size_t index, T value) {
    if(index < vector->length) {
        vector->buffer[index] = value;
        return;
    }

    if(index >= vector->capacity) {
        //(capacity * 2^k) = index + 1
        // k = log 2((index + 1) / capacity)
        //using long double to not lose precision when casting from size_t
        const long double k = log2l((long double)(index + 1) / (long double)vector->capacity);
        VECTOR_RESIZE(vector, (size_t)((long double)vector->capacity * powl(2.0L, ceill(k))));
    }

    memset(vector->buffer + vector->length, 0, sizeof(T) * (index - vector->length));
    vector->length = index + 1;
    vector->buffer[index] = value;
}

T VECTOR_REMOVE(VECTOR *const vector, const size_t index) {
    assert(index < vector->length);

    T ret = vector->buffer[index];
    memmove(vector->buffer + index, vector->buffer + index + 1, sizeof(T) * (vector->length - index));
    vector->length--;

    return ret;
}

#if defined VECTOR_PRINT_FORMAT && defined VECTOR_PRINT_ARGUMENTS
void VECTOR_PRINT(const VECTOR *const vector) {
    if(vector->length == 0) {
        puts("[]");
        return;
    }

    putchar('[');

    for(size_t i=0; i<vector->length - 1; i++) {
        printf(VECTOR_PRINT_FORMAT ", ", VECTOR_PRINT_ARGUMENTS(vector->buffer[i]));
    }

    if(vector->length > 1) {
        printf(VECTOR_PRINT_FORMAT, VECTOR_PRINT_ARGUMENTS(vector->buffer[vector->length - 1]));
    }

    puts("]");
}

char *VECTOR_TO_STRING(const VECTOR *const vector) {
    if(vector->length == 0) {
        return strdup("[]");
    }

    size_t length = 2;
    for(size_t i=0; i<vector->length - 1; i++) {
        const int char_printed = snprintf(NULL, 0, VECTOR_PRINT_FORMAT ", ", VECTOR_PRINT_ARGUMENTS(vector->buffer[i]));
        length += (size_t)char_printed;
    }
    if(vector->length > 1) {
        const int char_printed = snprintf(NULL, 0, VECTOR_PRINT_FORMAT, VECTOR_PRINT_ARGUMENTS(vector->buffer[vector->length - 1]));
        length += (size_t)char_printed;
    }

    char *vector_str = malloc(length * sizeof(char) + 1);
    assert(vector_str != NULL);
    vector_str[0] = '[';

    size_t offset = 1;
    for(size_t i=0; i<vector->length - 1; i++) {
        const int char_printed = sprintf(vector_str + offset, VECTOR_PRINT_FORMAT ", ", VECTOR_PRINT_ARGUMENTS(vector->buffer[i]));
        offset += (size_t)char_printed;
    }

    if(vector->length > 1) {
        sprintf(vector_str + offset, VECTOR_PRINT_FORMAT, VECTOR_PRINT_ARGUMENTS(vector->buffer[vector->length - 1]));
    }

    vector_str[length * sizeof(char) - 1] = ']'; 
    vector_str[length * sizeof(char)] = '\0'; 

    return vector_str;
}
#endif //VECTOR_PRINT_FORMAT VECTOR_PRINT_ARGUMENTS

#endif //VECTOR_IMPLEMENTATION

#undef T
#undef VECTOR_DEFAULT_CAPACITY
#undef VECTOR
#undef PREFIX
#undef VECTOR_INIT
#undef VECTOR_INIT_MOVE
#undef VECTOR_INIT_COPY
#undef VECTOR_COPY
#undef VECTOR_FREE
#undef VECTOR_PUSH
#undef VECTOR_POP
#undef VECTOR_AT
#undef VECTOR_SET
#undef VECTOR_REMOVE

#if defined VECTOR_PRINT_FORMAT && defined VECTOR_PRINT_ARGUMENTS
#undef VECTOR_PRINT
#undef VECTOR_TO_STRING
#undef VECTOR_PRINT_FORMAT
#undef VECTOR_PRINT_ARGUMENTS
#endif

#ifdef VECTOR_IMPLEMENTATION
#undef VECTOR_IMPLEMENTATION
#endif