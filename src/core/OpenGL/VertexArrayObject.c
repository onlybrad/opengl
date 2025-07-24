#include <assert.h>
#include <glad/glad.h>
#include <stdlib.h>
#include "VertexArrayObject.h"
#include "../Util/util.h"

void VertexArrayObject_init(VertexArrayObject *vao) {
    assert(vao != NULL);

    glGenVertexArrays(1, &vao->id);
}

void VertexArrayObject_free(VertexArrayObject *vao) {
    assert(vao != NULL);

    glDeleteVertexArrays(1, &vao->id);
}

void VertexArrayObject_add_buffer(VertexArrayObject *vao, const VertexArrayBuffer *vab, const IndexBuffer *ib, const VertexLayout *layout) {
    assert(vao != NULL);
    assert(vab != NULL);
    assert(layout != NULL);

    VertexArrayObject_bind(vao);
    VertexArrayBuffer_bind(vab);
    
    if(ib != NULL) {
        IndexBuffer_bind(ib);
    }

    int offset = 0;

    for(unsigned int i = 0; i < layout->attributes_index; i++) {
        const VertexAttribute *const attribute = layout->attributes + i;
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, (int)attribute->count, GL_FLOAT, attribute->normalized, layout->stride, INT_TO_PTR(offset));
        offset += (int)attribute->count * (int)sizeof(GLfloat);
    }
}

void VertexArrayObject_bind(const VertexArrayObject *vao) {
    assert(vao != NULL);

    glBindVertexArray(vao->id);
}

void VertexArrayObject_unbind(const VertexArrayObject *vao) {
    assert(vao != NULL);

    (void)vao;
    glBindVertexArray(0u);
}
