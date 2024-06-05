#ifndef _SHADER_H
#define _SHADER_H

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <stdbool.h>
#include "../Util/Str.h"

typedef const char *GLstring;
typedef struct Location {
    int location;
    char *uniform;
} Location;

#define K GLstring
#define V int
#include "../Template/Hashmap.h"
typedef struct Shader {
    unsigned int id;
    const char *vertex_shader_src;
    const char *fragment_shader_src;
    Hashmap_GLstring_int location_cache;
} Shader;

bool Shader_init(Shader *const shader, const char *const vertex_shader_path, const char *const fragment_shader_path);
void Shader_free(Shader *const shader);
void Shader_use(const Shader *const shader);
int Shader_get_location(Shader *const shader, const char *const name);
bool Shader_set_bool(Shader *const shader, const char *const name, const bool value);
bool Shader_set_int(Shader *const shader, const char *const name, const int value);
bool Shader_set_float(Shader *const shader, const char *const name, const float value);
bool Shader_set_mat4(Shader *const shader, const char *const name, const float *value);
bool Shader_set_vec3(Shader *const shader, const char *const name, const float *value);
bool Shader_set_vec4(Shader *const shader, const char *const name, const float *value);
void Shader_set_bool_l(Shader *const shader, const int location, const bool value);
void Shader_set_int_l(Shader *const shader, const int location, const int value);
void Shader_set_float_l(Shader *const shader, const int location, const float value);
void Shader_set_mat4_l(Shader *const shader, const int location, const float *value);
void Shader_set_vec3_l(Shader *const shader, const int location, const float *value);
void Shader_set_vec4_l(Shader *const shader, const int location, const float *value);

#endif