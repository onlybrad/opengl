#include <assert.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Shader.h"
#include "../Util/util.h"

//Hashmap_str_int
#define HASHMAP_K str
#define HASHMAP_V int
#define HASHMAP_IMPLEMENTATION
#include "../Template/Hashmap.h"

static unsigned OB_compile_shader(struct OB_String shader_source, GLenum shader_type) {
    const unsigned shader = glCreateShader(shader_type);
    int length = (int)shader_source.length;
    glShaderSource(shader, 1, &shader_source.buffer, &length);
    glCompileShader(shader);

    int success;
    char info_log[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        const char *shader_type_name;
        switch(shader_type) {
            case GL_VERTEX_SHADER: shader_type_name = "VERTEX_SHADER"; break;
            case GL_FRAGMENT_SHADER: shader_type_name = "FRAGMENT_SHADER"; break;
            default: shader_type_name = "UNKNOWN_SHADER"; break;
        }

        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr,"ERROR::%s::COMPILATION_FAILED\n%s\n", shader_type_name, info_log);
        return 0u;
    }

    return shader;
}

static unsigned OB_create_shader(struct OB_String vertex_shader_src, struct OB_String fragment_shader_src) {
    const unsigned vertex_shader = OB_compile_shader(vertex_shader_src, GL_VERTEX_SHADER);
    if(vertex_shader == 0u) {
        return 0u;
    }

    const unsigned fragment_shader = OB_compile_shader(fragment_shader_src, GL_FRAGMENT_SHADER);
    if(fragment_shader == 0u) {
        return 0u;
    }

    const unsigned program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "Shader Error: %s\n", infoLog);
        return 0u;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

bool OB_Shader_init(struct OB_Shader *shader, const char *vertex_shader_path, const char *fragment_shader_path) {
    assert(shader != NULL);
    assert(vertex_shader_path != NULL);
    assert(fragment_shader_path != NULL);

    const struct OB_String vertex_shader_src = OB_file_get_contents(vertex_shader_path);
    if(vertex_shader_src.buffer == NULL) {
        return false;
    }

    const struct OB_String fragment_shader_src = OB_file_get_contents(fragment_shader_path);
    if(fragment_shader_src.buffer == NULL) {
        OB_String_free(vertex_shader_src);
        return false;
    }

    const unsigned id = OB_create_shader(vertex_shader_src, fragment_shader_src);
    if(id == 0) {
        OB_String_free(vertex_shader_src);
        OB_String_free(fragment_shader_src);
        return false;
    }

    shader->id = id;
    shader->vertex_shader_src = vertex_shader_src;
    shader->fragment_shader_src = fragment_shader_src;
    if(!Hashmap_str_int_init(&shader->location_cache, OB_String_hash_function, OB_String_compare)) {
        OB_Shader_free(shader);
        return false;
    }

    return true;
}

void OB_Shader_free(struct OB_Shader *shader) {
    assert(shader != NULL);

    glDeleteProgram(shader->id);
    OB_String_free(shader->vertex_shader_src);
    OB_String_free(shader->fragment_shader_src);
    Hashmap_str_int_free(&shader->location_cache);
    memset(shader, 0, sizeof(*shader));
}

void OB_Shader_use(const struct OB_Shader *shader) {
    assert(shader != NULL);

    glUseProgram(shader->id);
}

int OB_Shader_get_location(struct OB_Shader *shader, const char *name) {
    assert(shader != NULL);
    assert(name != NULL);

    bool success;
    const int result = Hashmap_str_int_get(&shader->location_cache, name, &success);
    if(success) {
        return result;
    }

    const int location = glGetUniformLocation(shader->id, name);
    if(location < 0) {
        fprintf(stderr, "Could not find \"%s\" uniform in shader\n", name);
        return location;
    }

    if(!Hashmap_str_int_insert(&shader->location_cache, name, location)) {
        return -1;
    }

    return location;
}

bool OB_Shader_set_bool(struct OB_Shader *shader, const char *name, bool value) {
    assert(shader != NULL);
    assert(name != NULL);

    const int location = OB_Shader_get_location(shader, name);
    if(location < 0) {
        return false;
    }

    glUniform1i(location, (int)value);
    
    return true;
}

bool OB_Shader_set_int(struct OB_Shader *shader, const char *name, int value) {
    assert(shader != NULL);
    assert(name != NULL);

    const int location = OB_Shader_get_location(shader, name);
    if(location < 0) {
        return false;
    }

    glUniform1i(location, value);
    
    return true;
}

bool OB_Shader_set_float(struct OB_Shader *shader, const char *name, float value) {
    assert(shader != NULL);
    assert(name != NULL);

    const int location = OB_Shader_get_location(shader, name);
    if(location < 0) {
        return false;
    }

    glUniform1f(location, value);
    
    return true;
}

bool OB_Shader_set_mat4(struct OB_Shader *shader, const char *name, const float *value) {
    assert(shader != NULL);
    assert(name != NULL);
    assert(value != NULL);

    const int location = OB_Shader_get_location(shader, name);
    if(location < 0) {
        return false;
    }

    glUniformMatrix4fv(location, 1, false, value);

    return true;
}

bool OB_Shader_set_vec3(struct OB_Shader *shader, const char *name, const float *value) {
    assert(shader != NULL);
    assert(name != NULL);
    assert(value != NULL);

    const int location = OB_Shader_get_location(shader, name);
    if(location < 0) {
        return false;
    }

    glUniform3fv(location, 1, value);

    return true; 
}

bool OB_Shader_set_vec4(struct OB_Shader *shader, const char *name, const float *value) {
    assert(shader != NULL);
    assert(name != NULL);
    assert(value != NULL);

    const int location = OB_Shader_get_location(shader, name);
    if(location < 0) {
        return false;
    }

    glUniform4fv(location, 1, value);

    return true; 
}

inline void OB_Shader_set_bool_l(struct OB_Shader *shader, int location, bool value) {
    assert(shader != NULL);

    (void)shader;
    glUniform1i(location, (int)value);
}

inline void OB_Shader_set_int_l(struct OB_Shader *shader, int location, int value) {
    assert(shader != NULL);

    (void)shader;
    glUniform1i(location, value);    
}

inline void OB_Shader_set_float_l(struct OB_Shader *shader, int location, float value) {
    assert(shader != NULL);

    (void)shader;    
    glUniform1f(location, value);
}

inline void OB_Shader_set_mat4_l(struct OB_Shader *shader, int location, const float *value) {
    assert(shader != NULL);

    (void)shader;
    glUniformMatrix4fv(location, 1, false, value);
}

inline void OB_Shader_set_vec3_l(struct OB_Shader *shader, int location, const float *value) {
    assert(shader != NULL);

    (void)shader;
    glUniform3fv(location, 1, value);
}

inline void OB_Shader_set_vec4_l(struct OB_Shader *shader, int location, const float *value) {
    assert(shader != NULL);
    
    (void)shader;
    glUniform4fv(location, 1, value);
}