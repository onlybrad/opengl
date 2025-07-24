#include <stdio.h>
#include <stdlib.h>
#include "OpenGL.h"
#include "VertexLayout.h"
#include "../Util/util.h"
#include "../Object/Object.h"

int OB_OpenGL_width;
int OB_OpenGL_height;
int OB_MAX_VERTEX_ATTRIBS;
int OB_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
int OB_MAX_TEXTURE_IMAGE_UNITS;
struct OB_VertexLayout OB_OBJECT_VERTEX_LAYOUT;

static void OB_opengl_defaults_init(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

static void OB_max_values_init(void) {
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &OB_MAX_VERTEX_ATTRIBS);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &OB_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &OB_MAX_TEXTURE_IMAGE_UNITS);
}

static void OB_vertex_layout_init(void) {
    struct OB_VertexAttribute attributes[] = {
        {OB_ARRAY_MEMBER_LEN(struct OB_Vertex, position), false},
        {OB_ARRAY_MEMBER_LEN(struct OB_Vertex, normal), false},
        {OB_ARRAY_MEMBER_LEN(struct OB_Vertex, color), false},
        {OB_ARRAY_MEMBER_LEN(struct OB_Vertex, texture_coordinate), false},
        {OB_ARRAY_MEMBER_LEN(struct OB_Vertex, texture_slot), false},
        {OB_ARRAY_MEMBER_LEN(struct OB_Vertex, shininess), false},
        {OB_ARRAY_MEMBER_LEN(struct OB_Vertex, is_light), false},
        {OB_ARRAY_MEMBER_LEN(struct OB_Vertex, model), false}
    };

    OB_VertexLayout_init(&OB_OBJECT_VERTEX_LAYOUT);

    for(int i = 0; i < (int)OB_ARRAY_LEN(attributes); i++) {
        OB_VertexLayout_push(&OB_OBJECT_VERTEX_LAYOUT, attributes[i]);
    }
}

void OB_OpenGL_init(GLADloadproc proc_address, void (terminate)(void)) {
    if (! gladLoadGLLoader(proc_address)) {
        fputs("Failed to load OpenGL functions",stderr);
        terminate();
        exit(1);
    }

    OB_opengl_defaults_init();
    OB_max_values_init();
    OB_vertex_layout_init();
}

inline void OB_OpenGL_set_width(int width) {
    OB_OpenGL_width = width;
    glViewport(0, 0, width, OB_OpenGL_height);
}

inline void OB_OpenGL_set_height(int height) {
    OB_OpenGL_height = height;
    glViewport(0, 0, OB_OpenGL_width, height);    
}

inline void OB_OpenGL_set_viewport(int width, int height) {
    OB_OpenGL_width = width;
    OB_OpenGL_height = height;
    glViewport(0, 0, width, height);   
}

inline void OB_OpenGL_clear_color(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

inline void OB_OpenGL_clear(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void OB_OpenGL_fill_mode(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void OB_OpenGL_wireframe_mode(void) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

inline void OB_OpenGL_draw_background(int first, int vertices_count) {
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, first, vertices_count);
    glEnable(GL_DEPTH_TEST);
}

inline void OB_OpenGL_draw(int first, int vertices_count) {
    glDrawArrays(GL_TRIANGLES, first, vertices_count);
}

void OB_OpenGL_print_errors(void) {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL Error: %i", err);
    }
}