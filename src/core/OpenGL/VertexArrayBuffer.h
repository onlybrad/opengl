#ifndef _VERTEX_ARRAY_BUFFER_H
#define _VERTEX_ARRAY_BUFFER_H

#include <stdlib.h>
#include "../Thread/Thread.h"

typedef struct VertexArrayBuffer {
    unsigned id;
    size_t index;
    Lock lock;
} VertexArrayBuffer;

void VertexArrayBuffer_init(VertexArrayBuffer *vab, const void *data, const unsigned size);
void VertexArrayBuffer_push(VertexArrayBuffer *vab, const void *data, const unsigned size);
void VertexArrayBuffer_set(VertexArrayBuffer *vab, const unsigned index, const void *data, const unsigned size);
void VertexArrayBuffer_free(VertexArrayBuffer *vab);
void VertexArrayBuffer_bind(const VertexArrayBuffer *vab);
void VertexArrayBuffer_unbind(const VertexArrayBuffer *vab);

#endif