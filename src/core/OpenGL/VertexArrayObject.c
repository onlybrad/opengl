#include "VertexArrayObject.h"
#include "../util.h"

void VertexArrayObject_init(VertexArrayObject *const vao) {
    glGenVertexArrays(1, &vao->id);
}

void VertexArrayObject_free(VertexArrayObject *const vao) {
    glDeleteVertexArrays(1, &vao->id);
}

void VertexArrayObject_add_buffer(VertexArrayObject *const vao, const VertexArrayBuffer *const vab, const IndexBuffer *const ib, const VertexLayout *const layout) {
    VertexArrayObject_bind(vao);
    VertexArrayBuffer_bind(vab);
    
    if(ib != NULL) {
        IndexBuffer_bind(ib);
    }

    GLint offset = 0;

    for(GLuint i=0; i<layout->attributes_index; i++) {
        const VertexAttribute *const attribute = layout->attributes + i;
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, attribute->count, GL_FLOAT, attribute->normalized, layout->stride, INT_TO_PTR(offset));
        offset += attribute->count * (GLint)sizeof(GLfloat);
    }
}

void VertexArrayObject_bind(const VertexArrayObject *const vao) {
    glBindVertexArray(vao->id);
}

void VertexArrayObject_unbind(const VertexArrayObject *const vao) {
    (void)vao;
    glBindVertexArray(0);
}
