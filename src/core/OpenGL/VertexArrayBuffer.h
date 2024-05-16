#ifndef _VERTEX_ARRAY_BUFFER_H
#define _VERTEX_ARRAY_BUFFER_H

#include <glad/glad.h>
#include <stdlib.h>
#include "../Thread/Thread.h"

typedef struct VertexArrayBuffer {
    unsigned int id;
    size_t index;
    Lock lock;
} VertexArrayBuffer;

void VertexArrayBuffer_init(VertexArrayBuffer *const vab, const void *const data, const unsigned int size);
void VertexArrayBuffer_push(VertexArrayBuffer *const vab, const void *const data, const unsigned int size);
void VertexArrayBuffer_set(VertexArrayBuffer *const vab, const unsigned int index, const void *const data, const unsigned int size);
void VertexArrayBuffer_free(VertexArrayBuffer *const vab);
void VertexArrayBuffer_bind(const VertexArrayBuffer *const vab);
void VertexArrayBuffer_unbind(const VertexArrayBuffer *const vab);

#endif