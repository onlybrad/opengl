#ifndef _VERTEX_ARRAY_OBJECT_H
#define _VERTEX_ARRAY_OBJECT_H

#include <glad/glad.h>
#include "VertexArrayBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"

typedef struct VertexArrayObject {
    unsigned int id;
} VertexArrayObject;

void VertexArrayObject_init(VertexArrayObject *const vao);
void VertexArrayObject_free(VertexArrayObject *const vao);
void VertexArrayObject_add_buffer(VertexArrayObject *const vao, const VertexArrayBuffer *const vab, const IndexBuffer *const ib, const VertexLayout *const layout);
void VertexArrayObject_bind(const VertexArrayObject *const vao);
void VertexArrayObject_unbind(const VertexArrayObject *const vao);

#endif