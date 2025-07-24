#ifndef _VERTEX_ARRAY_OBJECT_H
#define _VERTEX_ARRAY_OBJECT_H

#include "VertexArrayBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"

typedef struct VertexArrayObject {
    unsigned id;
} VertexArrayObject;

void VertexArrayObject_init(VertexArrayObject *vao);
void VertexArrayObject_free(VertexArrayObject *vao);
void VertexArrayObject_add_buffer(VertexArrayObject *vao, const VertexArrayBuffer *vab, const IndexBuffer *ib, const VertexLayout *layout);
void VertexArrayObject_bind(const VertexArrayObject *vao);
void VertexArrayObject_unbind(const VertexArrayObject *vao);

#endif