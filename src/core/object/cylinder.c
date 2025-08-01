#include <assert.h>
#include <cglm/cglm.h>
#include <stdlib.h>
#include "cylinder.h"

static struct OB_Object_Vertex *OB_generate_cylinder_vertices(unsigned segments, float radius, float height, size_t *size) {
    const float π = 3.141593f;
    const float step = 2.0f * π / (float)segments;
    const size_t vertex_count = (size_t)segments * 12;

    *size = vertex_count * sizeof(struct OB_Object_Vertex);
    struct OB_Object_Vertex *vertices = (struct OB_Object_Vertex*)calloc(vertex_count, sizeof(struct OB_Object_Vertex));
    if(vertices == NULL) {
        return NULL;
    }

    struct OB_Object_Vertex *vertex = vertices;
    for (unsigned i = 0u; i < segments; i++) {
        const float θ0 = (float)i * step;
        const float θ1 = (float)(i + 1) * step;

        const float x0 = radius * cosf(θ0);
        const float z0 = radius * sinf(θ0);
        const float x1 = radius * cosf(θ1);
        const float z1 = radius * sinf(θ1);

        const float nx0 = cosf(θ0);
        const float nz0 = sinf(θ0);
        const float nx1 = cosf(θ1);
        const float nz1 = sinf(θ1);

        const float u0 = (float)i / (float)segments;
        const float u1 = (float)(i + 1) / (float)segments;
        
        vertex->position[0] = x0;
        vertex->position[1] = height;
        vertex->position[2] = z0;
        vertex->normal[0] = nx0;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = nz0;
        vertex->texture_coordinate[0] = u0;
        vertex->texture_coordinate[1] = 0.0f;

        vertex++;

        vertex->position[0] = x1;
        vertex->position[1] = height;
        vertex->position[2] = z1;
        vertex->normal[0] = nx1;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = nz1;
        vertex->texture_coordinate[0] = u1;
        vertex->texture_coordinate[1] = 0.0f;

        vertex++;

        vertex->position[0] = x0;
        vertex->position[1] = -height;
        vertex->position[2] = z0;
        vertex->normal[0] = nx0;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = nz0;
        vertex->texture_coordinate[0] = u0;
        vertex->texture_coordinate[1] = 1.0f;

        vertex++;

        // === OUTSIDE triangle 2 ===
        vertex->position[0] = x1;
        vertex->position[1] = height;
        vertex->position[2] = z1;
        vertex->normal[0] = nx1;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = nz1;
        vertex->texture_coordinate[0] = u1;
        vertex->texture_coordinate[1] = 0.0f;
        vertex++;

        vertex->position[0] = x1;
        vertex->position[1] = -height;
        vertex->position[2] = z1;
        vertex->normal[0] = nx1;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = nz1;
        vertex->texture_coordinate[0] = u1;
        vertex->texture_coordinate[1] = 1.0f;
        vertex++;

        vertex->position[0] = x0;
        vertex->position[1] = -height;
        vertex->position[2] = z0;
        vertex->normal[0] = nx0;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = nz0;
        vertex->texture_coordinate[0] = u0;
        vertex->texture_coordinate[1] = 1.0f;

        vertex++;

        // === INSIDE triangle 1 ===
        vertex->position[0] = x0;
        vertex->position[1] = height;
        vertex->position[2] = z0;
        vertex->normal[0] = -nx0;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = -nz0;
        vertex->texture_coordinate[0] = u0;
        vertex->texture_coordinate[1] = 0.0f;

        vertex++;

        vertex->position[0] = x0;
        vertex->position[1] = -height;
        vertex->position[2] = z0;
        vertex->normal[0] = -nx0;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = -nz0;
        vertex->texture_coordinate[0] = u0;
        vertex->texture_coordinate[1] = 1.0f;

        vertex++;

        vertex->position[0] = x1;
        vertex->position[1] = height;
        vertex->position[2] = z1;
        vertex->normal[0] = -nx1;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = -nz1;
        vertex->texture_coordinate[0] = u1;
        vertex->texture_coordinate[1] = 0.0f;

        vertex++;

        // === INSIDE triangle 2 ===
        vertex->position[0] = x1;
        vertex->position[1] = height;
        vertex->position[2] = z1;
        vertex->normal[0] = -nx1;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = -nz1;
        vertex->texture_coordinate[0] = u1;
        vertex->texture_coordinate[1] = 0.0f;

        vertex++;

        vertex->position[0] = x0;
        vertex->position[1] = -height;
        vertex->position[2] = z0;
        vertex->normal[0] = -nx0;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = -nz0;
        vertex->texture_coordinate[0] = u0;
        vertex->texture_coordinate[1] = 1.0f;

        vertex++;

        vertex->position[0] = x1;
        vertex->position[1] = -height;
        vertex->position[2] = z1;
        vertex->normal[0] = -nx1;
        vertex->normal[1] = 0.0f;
        vertex->normal[2] = -nz1;
        vertex->texture_coordinate[0] = u1;
        vertex->texture_coordinate[1] = 1.0f;

        vertex++;
    }

    return vertices;
}

bool OB_Cylinder_create(struct OB_Object *object, float radius, float height) {
    assert(object != NULL);

    size_t size;
    struct OB_Object_Vertex *cylinder_vertices = OB_generate_cylinder_vertices(20u, radius, height, &size);
    if(cylinder_vertices == NULL) {
        return false;
    }

    OB_Object_init_move(object, cylinder_vertices, (unsigned)(size/sizeof(*cylinder_vertices)));
    mat4 model;
    glm_mat4_identity(model);
    OB_Object_set_model(object, (float*)model);
    OB_Object_set_shininess(object, 32u);

    return true;
}
