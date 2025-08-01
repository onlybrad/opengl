#ifndef OB_OPENGL_H
#define OB_OPENGL_H

#include <stdbool.h>
#include <glad/glad.h>

bool OB_OpenGL_init(GLADloadproc proc_address);
void OB_OpenGL_set_width(int width);
void OB_OpenGL_set_height(int height);
void OB_OpenGL_set_viewport(int width, int height);
void OB_OpenGL_clear_color(float r, float g, float b, float a);
void OB_OpenGL_clear(void);
void OB_OpenGL_fill_mode(void);
void OB_OpenGL_wireframe_mode(void);
void OB_OpenGL_draw_background(int first, int vertices_count);
void OB_OpenGL_draw(int first, int vertices_count);
void OB_OpenGL_print_errors(void);

#endif