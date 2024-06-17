#ifndef _VERTEX_LAYOUT
#define _VERTEX_LAYOUT

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

void VertexLayout_init(VertexLayout layout[static 1]);
bool VertexLayout_push(VertexLayout layout[static 1], const VertexAttribute attribute);

#endif