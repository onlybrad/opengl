#ifndef _VERTEX_LAYOUT
#define _VERTEX_LAYOUT

#include <glad/glad.h>
#include <stdbool.h>

typedef struct VertexAttribute {
    unsigned int count;
    bool normalized;
} VertexAttribute;

typedef struct VertexLayout {
    VertexAttribute attributes[32];
    unsigned int attributes_index;
    int stride;
} VertexLayout;

void VertexLayout_init(VertexLayout *const layout);
bool VertexLayout_push(VertexLayout *const layout, const VertexAttribute attribute);

#endif