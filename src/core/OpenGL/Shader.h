#ifndef _SHADER_H
#define _SHADER_H

#include <cglm/cglm.h>
#include <stdbool.h>
#include "../Util/String.h"

typedef struct Location {
    int location;
    char *uniform;
} Location;

typedef const char *str;
#define K str
#define VALUE int
#include "../Template/HashMap.h"
typedef struct Shader {
    unsigned int id;
    String vertex_shader_src;
    String fragment_shader_src;
    HashMap_str_int location_cache;
} Shader;

bool Shader_init(Shader *shader, const char *vertex_shader_path, const char *fragment_shader_path);
void Shader_free(Shader *shader);
void Shader_use(const Shader *shader);
int Shader_get_location(Shader *shader, const char *name);
bool Shader_set_bool(Shader *shader, const char *name, const bool value);
bool Shader_set_int(Shader *shader, const char *name, const int value);
bool Shader_set_float(Shader *shader, const char *name, const float value);
bool Shader_set_mat4(Shader *shader, const char *name, const float *value);
bool Shader_set_vec3(Shader *shader, const char *name, const float *value);
bool Shader_set_vec4(Shader *shader, const char *name, const float *value);
void Shader_set_bool_l(Shader *shader, const int location, const bool value);
void Shader_set_int_l(Shader *shader, const int location, const int value);
void Shader_set_float_l(Shader *shader, const int location, const float value);
void Shader_set_mat4_l(Shader *shader, const int location, const float *value);
void Shader_set_vec3_l(Shader *shader, const int location, const float *value);
void Shader_set_vec4_l(Shader *shader, const int location, const float *value);

#endif