#include <stdlib.h>
#include "../util/util.h"

#ifndef VECTOR_T
    #error "Missing parameter VECTOR_T"
#endif

#define VECTOR_CAPACITY  16
#define VECTOR           OB_MAKE_NAME(Vector, VECTOR_T)
#define VECTOR_RESIZE    OB_MAKE_NAME(VECTOR, resize)
#define VECTOR_INIT      OB_MAKE_NAME(VECTOR, init)
#define VECTOR_INIT_MOVE OB_MAKE_NAME(VECTOR, init_move)
#define VECTOR_INIT_COPY OB_MAKE_NAME(VECTOR, init_copy)
#define VECTOR_COPY      OB_MAKE_NAME(VECTOR, copy)
#define VECTOR_FREE      OB_MAKE_NAME(VECTOR, free)
#define VECTOR_PUSH      OB_MAKE_NAME(VECTOR, push)
#define VECTOR_POP       OB_MAKE_NAME(VECTOR, pop)
#define VECTOR_AT        OB_MAKE_NAME(VECTOR, at)
#define VECTOR_SET       OB_MAKE_NAME(VECTOR, set)
#define VECTOR_REMOVE    OB_MAKE_NAME(VECTOR, remove)

#ifndef VECTOR_IMPLEMENTATION

struct VECTOR {
    VECTOR_T *data;
    size_t length;
    size_t capacity;
};

bool VECTOR_INIT      (struct VECTOR *vector, size_t capacity);
void VECTOR_INIT_MOVE (struct VECTOR *vector, VECTOR_T *data, size_t length);
bool VECTOR_INIT_COPY (struct VECTOR *vector, VECTOR_T *data, size_t length);
bool VECTOR_COPY      (struct VECTOR *vector_dst, struct VECTOR *vector_src);
void VECTOR_FREE      (struct VECTOR *vector);
bool VECTOR_PUSH      (struct VECTOR *vector, VECTOR_T value);
VECTOR_T VECTOR_POP   (struct VECTOR *vector);
VECTOR_T VECTOR_AT    (const struct VECTOR *vector, size_t index);
bool VECTOR_SET       (struct VECTOR *vector, size_t index, VECTOR_T value);
VECTOR_T VECTOR_REMOVE(struct VECTOR *vector, size_t index);

#endif //#ifndef VECTOR_IMPLEMENTATION

#ifdef VECTOR_IMPLEMENTATION 

#include <assert.h>
//#include <math.h>
#include <string.h>

static bool VECTOR_RESIZE(struct VECTOR *vector, size_t new_capacity) {
    VECTOR_T *data = (VECTOR_T*)realloc(vector->data, sizeof(VECTOR_T) * new_capacity);
    if(data == NULL) {
        return false;
    }

    vector->data = data;
    vector->capacity = new_capacity;

    return true;
}

bool VECTOR_INIT(struct VECTOR *vector, size_t capacity) {
    vector->capacity = capacity == 0 ? VECTOR_CAPACITY : capacity;
    vector->length = 0;
    vector->data = (VECTOR_T*)calloc(vector->capacity, sizeof(VECTOR_T));
    
    return vector->data != NULL;
}

void VECTOR_INIT_MOVE(struct VECTOR *vector, VECTOR_T *data, size_t length) {
    vector->capacity = length;
    vector->length = length;
    vector->data = data;
}

bool VECTOR_INIT_COPY(struct VECTOR *vector, VECTOR_T *data, size_t length) {
    vector->capacity = length;
    vector->length = length;
    vector->data = (VECTOR_T*)malloc(sizeof(VECTOR_T) * length);
    
    if(vector->data == NULL) {
        return false;
    }

    memcpy(vector->data, data, sizeof(VECTOR_T) * length);

    return true;
}

void VECTOR_FREE(struct VECTOR *vector) {
    free(vector->data);
    memset(vector, 0, sizeof(*vector));
}

bool VECTOR_COPY(struct VECTOR *vector_dst, struct VECTOR *vector_src) {
    vector_dst->data = (VECTOR_T*)malloc(sizeof(VECTOR_T) * vector_src->capacity);

    if(vector_dst->data == NULL) {
        return false;
    }

    memcpy(vector_dst->data, vector_src->data, sizeof(VECTOR_T) * vector_src->capacity);
    vector_dst->capacity = vector_src->capacity;
    vector_dst->length = vector_src->length;

    return true;
}

bool VECTOR_PUSH(struct VECTOR *vector, VECTOR_T value) {
    if(vector->length == vector->capacity) {
        if(!VECTOR_RESIZE(vector, vector->capacity * 2)) {
            return false;
        }
    }
    vector->data[vector->length++] = value;

    return true;
}

VECTOR_T VECTOR_POP(struct VECTOR *vector) {
    assert(vector->length > 0);

    return vector->data[--vector->length];
}

VECTOR_T VECTOR_AT(const struct VECTOR *vector, size_t index) {
    assert(index < vector->length);

    return vector->data[index];
}

bool VECTOR_SET(struct VECTOR *vector, size_t index, VECTOR_T value) {
    if(index < vector->length) {
        vector->data[index] = value;
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

    memset(vector->data + vector->length, 0, sizeof(VECTOR_T) * (index - vector->length));
    vector->length = index + 1;
    vector->data[index] = value;

    return true;
}

VECTOR_T VECTOR_REMOVE(struct VECTOR *vector, size_t index) {
    assert(index < vector->length);

    VECTOR_T ret = vector->data[index];
    memmove(vector->data + index, vector->data + index + 1, sizeof(VECTOR_T) * (vector->length - index));
    vector->length--;

    return ret;
}

#endif //#ifdef VECTOR_IMPLEMENTATION

#undef VECTOR_T
#undef VECTOR_CAPACITY
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

#ifdef VECTOR_IMPLEMENTATION
#undef VECTOR_IMPLEMENTATION
#endif