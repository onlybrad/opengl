#include "VertexLayout.h"

extern GLint MAX_VERTEX_ATTRIBS;

void VertexLayout_init(VertexLayout *const layout) {
    layout->attributes_index = 0;
    layout->stride = 0;
}

GLboolean VertexLayout_push(VertexLayout *const layout, const VertexAttribute *const attribute) {
    if(layout->attributes_index == (GLuint)MAX_VERTEX_ATTRIBS) {
        return GL_FALSE;
    }

    layout->attributes[layout->attributes_index++] = *attribute;
    layout->stride += attribute->count * (GLint)sizeof(GLfloat);

    return GL_TRUE;
}