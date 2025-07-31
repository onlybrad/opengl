#ifndef OB_VERTEX_ARRAY_OBJECT_H
#define OB_VERTEX_ARRAY_OBJECT_H

#include "vertexarraybuffer.h"
#include "vertexlayout.h"
#include "indexbuffer.h"

struct OB_VertexArrayObject {
    unsigned id;
};

void OB_VertexArrayObject_init(struct OB_VertexArrayObject *vao);
void OB_VertexArrayObject_free(struct OB_VertexArrayObject *vao);
void OB_VertexArrayObject_add_buffer(
    struct OB_VertexArrayObject *vao,
    const struct OB_VertexArrayBuffer *vab,
    const struct OB_IndexBuffer *ib,
    const struct OB_VertexLayout *layout
);
void OB_VertexArrayObject_bind(const struct OB_VertexArrayObject *vao);
void OB_VertexArrayObject_unbind(const struct OB_VertexArrayObject *vao);

#endif