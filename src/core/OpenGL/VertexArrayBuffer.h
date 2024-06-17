#ifndef _VERTEX_ARRAY_BUFFER_H
#define _VERTEX_ARRAY_BUFFER_H

#include <stdlib.h>
#include "../Thread/Thread.h"

typedef struct VertexArrayBuffer {
    unsigned int id;
    size_t index;
    Lock lock;
} VertexArrayBuffer;

void VertexArrayBuffer_init(VertexArrayBuffer vab[static 1], const void *const data, const unsigned int size);
void VertexArrayBuffer_push(VertexArrayBuffer vab[static 1], const void *const data, const unsigned int size);
void VertexArrayBuffer_set(VertexArrayBuffer vab[static 1], const unsigned int index, const void *const data, const unsigned int size);
void VertexArrayBuffer_free(VertexArrayBuffer vab[static 1]);
void VertexArrayBuffer_bind(const VertexArrayBuffer vab[static 1]);
void VertexArrayBuffer_unbind(const VertexArrayBuffer vab[static 1]);

#endif