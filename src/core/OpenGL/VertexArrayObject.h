#ifndef _VERTEX_ARRAY_OBJECT_H
#define _VERTEX_ARRAY_OBJECT_H

#include "VertexArrayBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"

typedef struct VertexArrayObject {
    unsigned int id;
} VertexArrayObject;

void VertexArrayObject_init(VertexArrayObject vao[static 1]);
void VertexArrayObject_free(VertexArrayObject vao[static 1]);
void VertexArrayObject_add_buffer(VertexArrayObject vao[static 1], const VertexArrayBuffer *const vab, const IndexBuffer *const ib, const VertexLayout *const layout);
void VertexArrayObject_bind(const VertexArrayObject vao[static 1]);
void VertexArrayObject_unbind(const VertexArrayObject vao[static 1]);

#endif