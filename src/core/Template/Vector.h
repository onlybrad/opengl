#include <stdlib.h>
#include "../Util/util.h"

//this is for testing (IDE highlighting purpose)
#if 0
typedef const char *str;
#define T str 
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

bool VECTOR_INIT(VECTOR *vector, size_t capacity);
void VECTOR_INIT_MOVE(VECTOR *vector, T *buffer, size_t length);
bool VECTOR_INIT_COPY(VECTOR *vector, T *buffer, size_t length);
bool VECTOR_COPY(VECTOR *vector_dst, VECTOR *vector_src);
void VECTOR_FREE(VECTOR *vector);
bool VECTOR_PUSH(VECTOR *vector, T value);
T VECTOR_POP(VECTOR *vector);
T VECTOR_AT(const VECTOR *vector, size_t index);
bool VECTOR_SET(VECTOR *vector, size_t index, T value);
T VECTOR_REMOVE(VECTOR *vector, size_t index);

#if defined VECTOR_PRINT_FORMAT && defined VECTOR_PRINT_ARGUMENTS
void VECTOR_PRINT(const VECTOR *vector);
char *VECTOR_TO_STRING(const VECTOR *vector);
#endif //VECTOR_PRINT_FORMAT VECTOR_PRINT_ARGUMENTS

#endif //#ifndef VECTOR_IMPLEMENTATION

//this is for testing (IDE highlighting purpose)
#if 0
#define VECTOR_IMPLEMENTATION
#endif

#ifdef VECTOR_IMPLEMENTATION 

#include <assert.h>
//#include <math.h>
#include <string.h>

static bool VECTOR_RESIZE(VECTOR *vector, size_t new_capacity) {
    T *buffer = (T*)realloc(vector->buffer, sizeof(T) * new_capacity);
    if(buffer == NULL) {
        return false;
    }

    vector->buffer = buffer;
    vector->capacity = new_capacity;

    return true;
}

bool VECTOR_INIT(VECTOR *vector, size_t capacity) {
    vector->capacity = capacity == 0 ? VECTOR_DEFAULT_CAPACITY : capacity;
    vector->length = 0;
    vector->buffer = (T*)calloc(vector->capacity, sizeof(T));
    
    return vector->buffer != NULL;
}

void VECTOR_INIT_MOVE(VECTOR *vector, T *buffer, size_t length) {
    vector->capacity = length;
    vector->length = length;
    vector->buffer = buffer;
}

bool VECTOR_INIT_COPY(VECTOR *vector, T *buffer, size_t length) {
    vector->capacity = length;
    vector->length = length;
    vector->buffer = (T*)malloc(sizeof(T) * length);
    
    if(vector->buffer == NULL) {
        return false;
    }

    memcpy(vector->buffer, buffer, sizeof(T) * length);

    return true;
}

void VECTOR_FREE(VECTOR *vector) {
    free(vector->buffer);
    memset(vector, 0, sizeof(*vector));
}

bool VECTOR_COPY(VECTOR *vector_dst, VECTOR *vector_src) {
    free(vector_dst->buffer);
    vector_dst->buffer = (T*)malloc(sizeof(T) * vector_src->capacity);

    if(vector_dst->buffer == NULL) {
        return false;
    }

    memcpy(vector_dst->buffer, vector_src->buffer, sizeof(T) * vector_src->capacity);
    vector_dst->capacity = vector_src->capacity;
    vector_dst->length = vector_src->length;

    return true;
}

bool VECTOR_PUSH(VECTOR *vector, T value) {
    if(vector->length == vector->capacity) {
        if(!VECTOR_RESIZE(vector, vector->capacity * 2)) {
            return false;
        }
    }
    vector->buffer[vector->length++] = value;

    return true;
}

T VECTOR_POP(VECTOR *vector) {
    assert(vector->length > 0);

    return vector->buffer[--vector->length];
}

T VECTOR_AT(const VECTOR *vector, size_t index) {
    assert(index < vector->length);

    return vector->buffer[index];
}

bool VECTOR_SET(VECTOR *vector, size_t index, T value) {
    if(index < vector->length) {
        vector->buffer[index] = value;
        return true;
    }

    if(index >= vector->capacity) {
        size_t new_capacity = vector->capacity;

        do {
            new_capacity *= 2;
        } while(new_capacity <= index);

        if(!VECTOR_RESIZE(vector, new_capacity)) {
            return false;
        }
    }

    memset(vector->buffer + vector->length, 0, sizeof(T) * (index - vector->length));
    vector->length = index + 1;
    vector->buffer[index] = value;

    return true;
}

T VECTOR_REMOVE(VECTOR *vector, size_t index) {
    assert(index < vector->length);

    T ret = vector->buffer[index];
    memmove(vector->buffer + index, vector->buffer + index + 1, sizeof(T) * (vector->length - index));
    vector->length--;

    return ret;
}

#if defined VECTOR_PRINT_FORMAT && defined VECTOR_PRINT_ARGUMENTS
#include <stdio.h>
void VECTOR_PRINT(const VECTOR *vector) {
    if(vector->length == 0) {
        puts("[]");
        return;
    }

    putchar('[');

    for(size_t i = 0; i < vector->length - 1; i++) {
        printf(VECTOR_PRINT_FORMAT ", ", VECTOR_PRINT_ARGUMENTS(vector->buffer[i]));
    }

    if(vector->length > 1) {
        printf(VECTOR_PRINT_FORMAT, VECTOR_PRINT_ARGUMENTS(vector->buffer[vector->length - 1]));
    }

    puts("]");
}

char *VECTOR_TO_STRING(const VECTOR *vector) {
    if(vector->length == 0) {
        return strdup("[]");
    }

    size_t length = 2;
    for(size_t i = 0; i < vector->length - 1; i++) {
        const int char_printed = snprintf(NULL, 0, VECTOR_PRINT_FORMAT ", ", VECTOR_PRINT_ARGUMENTS(vector->buffer[i]));
        length += (size_t)char_printed;
    }
    if(vector->length > 1) {
        const int char_printed = snprintf(NULL, 0, VECTOR_PRINT_FORMAT, VECTOR_PRINT_ARGUMENTS(vector->buffer[vector->length - 1]));
        length += (size_t)char_printed;
    }

    char *vector_str = (char*)malloc((length + 1) * sizeof(char));
    assert(vector_str != NULL);
    vector_str[0] = '[';

    size_t offset = 1;
    for(size_t i = 0; i < vector->length - 1; i++) {
        const int char_printed = sprintf(vector_str + offset, VECTOR_PRINT_FORMAT ", ", VECTOR_PRINT_ARGUMENTS(vector->buffer[i]));
        offset += (size_t)char_printed;
    }

    if(vector->length > 1) {
        sprintf(vector_str + offset, VECTOR_PRINT_FORMAT, VECTOR_PRINT_ARGUMENTS(vector->buffer[vector->length - 1]));
    }

    vector_str[length - 1] = ']'; 
    vector_str[length] = '\0'; 

    return vector_str;
}
#endif //VECTOR_PRINT_FORMAT VECTOR_PRINT_ARGUMENTS

#endif //#ifdef VECTOR_IMPLEMENTATION

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