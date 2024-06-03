#ifndef _SHADER_H
#define _SHADER_H

#include <glad/glad.h>
#include <cglm/cglm.h>
#include "../Util/Str.h"

typedef const GLchar *GLstring;
typedef struct Location {
    GLint location;
    GLchar *uniform;
} Location;

#define K GLstring
#define V GLint
#include "../Template/Hashmap.h"
typedef struct Shader {
    GLuint id;
    const GLchar *vertex_shader_src;
    const GLchar *fragment_shader_src;
    Hashmap_GLstring_GLint location_cache;
} Shader;

GLboolean Shader_init(Shader *const shader, const GLchar *const vertex_shader_path, const GLchar *const fragment_shader_path);
void Shader_free(Shader *const shader);
void Shader_use(const Shader *const shader);
GLint Shader_get_location(Shader *const shader, const GLchar *const name);
GLboolean Shader_set_bool(Shader *const shader, const GLchar *const name, GLboolean value);
GLboolean Shader_set_int(Shader *const shader, const GLchar *const name, GLint value);
GLboolean Shader_set_float(Shader *const shader, const GLchar *const name, GLfloat value);
GLboolean Shader_set_mat4(Shader *const shader, const GLchar *const name, const GLfloat *value);
GLboolean Shader_set_vec3(Shader *const shader, const GLchar *const name, const GLfloat *value);
GLboolean Shader_set_vec4(Shader *const shader, const GLchar *const name, const GLfloat *value);
void Shader_set_bool_l(Shader *const shader, const GLint location, GLboolean value);
void Shader_set_int_l(Shader *const shader, const GLint location, GLint value);
void Shader_set_float_l(Shader *const shader, const GLint location, GLfloat value);
void Shader_set_mat4_l(Shader *const shader, const GLint location, const GLfloat *value);
void Shader_set_vec3_l(Shader *const shader, const GLint location, const GLfloat *value);
void Shader_set_vec4_l(Shader *const shader, const GLint location, const GLfloat *value);

#endif