#ifndef _VERTEX_LAYOUT
#define _VERTEX_LAYOUT

#include <glad/glad.h>

typedef struct VertexAttribute {
    GLint count;
    GLboolean normalized;
} VertexAttribute;

typedef struct VertexLayout {
    VertexAttribute attributes[32];
    GLuint attributes_index;
    GLsizei stride;
} VertexLayout;

void VertexLayout_init(VertexLayout *const layout);
GLboolean VertexLayout_push(VertexLayout *const layout, const VertexAttribute *const attribute);

#endif