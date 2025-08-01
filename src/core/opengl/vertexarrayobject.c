#include <assert.h>
#include <glad/glad.h>
#include <stdlib.h>
#include "vertexarrayobject.h"
#include "../util/util.h"

void OB_VertexArrayObject_init(struct OB_VertexArrayObject *vao) {
    assert(vao != NULL);

    glGenVertexArrays(1, &vao->id);
}

void OB_VertexArrayObject_free(struct OB_VertexArrayObject *vao) {
    assert(vao != NULL);

    glDeleteVertexArrays(1, &vao->id);
}

void OB_VertexArrayObject_add_buffer(
    struct OB_VertexArrayObject *vao,
    const struct OB_VertexArrayBuffer *vab,
    const struct OB_IndexBuffer *ib,
    const struct OB_VertexLayout *layout
) {
    assert(vao != NULL);
    assert(vab != NULL);
    assert(layout != NULL);

    OB_VertexArrayObject_bind(vao);
    OB_VertexArrayBuffer_bind(vab);
    
    if(ib != NULL) {
        OB_IndexBuffer_bind(ib);
    }

    int offset = 0;

    for(unsigned i = 0; i < layout->attributes_index; i++) {
        const struct OB_VertexAttribute *const attribute = layout->attributes + i;
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, (int)attribute->count, GL_FLOAT, attribute->normalized, layout->stride, OB_INT_TO_PTR(offset));
        offset += (int)attribute->count * (int)sizeof(GLfloat);
    }
}

void OB_VertexArrayObject_bind(const struct OB_VertexArrayObject *vao) {
    assert(vao != NULL);

    glBindVertexArray(vao->id);
}

void OB_VertexArrayObject_unbind(const struct OB_VertexArrayObject *vao) {
    assert(vao != NULL);

    (void)vao;
    glBindVertexArray(0u);
}
