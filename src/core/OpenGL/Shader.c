#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Shader.h"
#include "../Util/util.h"

static GLuint compile_shader(const GLchar *const shader_source, size_t length, const GLenum shader_type) {
    const GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, (GLint *)&length);
    glCompileShader(shader);

    GLint success;
    GLchar info_log[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        const GLchar *shader_type_name;
        switch(shader_type) {
            case GL_VERTEX_SHADER: shader_type_name = "VERTEX_SHADER"; break;
            case GL_FRAGMENT_SHADER: shader_type_name = "FRAGMENT_SHADER"; break;
            default: shader_type_name = "UNKNOWN_SHADER"; break;
        }

        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr,"ERROR::%s::COMPILATION_FAILED\n%s\n",shader_type_name, info_log);
        return 0;
    }

    return shader;
}

static GLuint create_shader(const GLchar *const vertex_shader_src, const size_t vertex_shader_length, const GLchar *const fragment_shader_src, const size_t fragment_shader_length) {
    const GLuint vertex_shader = compile_shader(vertex_shader_src, vertex_shader_length, GL_VERTEX_SHADER);
    if(vertex_shader == 0) {
        return 0;
    }

    const GLuint fragment_shader = compile_shader(fragment_shader_src, fragment_shader_length, GL_FRAGMENT_SHADER);
    if(fragment_shader == 0) {
        return 0;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr,"ERROR::PROGRAM::LINKING_FAILED\n%s\n",infoLog);
        return 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

GLboolean Shader_init(Shader *const shader, const GLchar *const vertex_shader_path, const GLchar *const fragment_shader_path) {
    size_t vertex_shader_length;
    const GLchar* const vertex_shader_src = file_get_contents(&vertex_shader_length, vertex_shader_path);
    if(vertex_shader_src == NULL) {
        return GL_FALSE;
    }

    size_t fragment_shader_length;
    const GLchar* const fragment_shader_src = file_get_contents(&fragment_shader_length, fragment_shader_path);
    if(fragment_shader_src == NULL) {
        free(unconst(vertex_shader_src));
        return GL_FALSE;
    }

    const GLuint id = create_shader(vertex_shader_src, vertex_shader_length, fragment_shader_src, fragment_shader_length);
    if(id == 0) {
        free(unconst(vertex_shader_src));
        free(unconst(fragment_shader_src));
        return GL_FALSE;
    }

    shader->id = id;
    shader->vertex_shader_src = vertex_shader_src;
    shader->fragment_shader_src = fragment_shader_src;
    shader->location_count = 0;
    memset(shader->location_cache, 0, sizeof(shader->location_cache));

    return GL_TRUE;
}

void Shader_free(Shader *const shader) {
    glDeleteProgram(shader->id);
    free(unconst(shader->fragment_shader_src));
    free(unconst(shader->vertex_shader_src));
    for(unsigned int i=0u; i<shader->location_count; i++) {
        free(unconst(shader->location_cache[i].uniform));
        shader->location_cache[i].uniform = NULL;
        shader->location_cache[i].location = 0;
    }
}

void Shader_use(const Shader *const shader) {
    glUseProgram(shader->id);
}

GLint Shader_get_location(Shader *const shader, const GLchar *const name) {
    for(GLuint i=0u; i<shader->location_count; i++) {
        if(shader->location_cache[i].uniform != NULL && strcmp(name, shader->location_cache[i].uniform) == 0) {
            return shader->location_cache[i].location;
        }
    }

    GLint location = glGetUniformLocation(shader->id, name);
    if(location < 0) {
        fprintf(stderr, "Could not find \"%s\" uniform in shader\n", name);
        return location;
    }

    shader->location_cache[shader->location_count].location = location;
    shader->location_cache[shader->location_count].uniform = strdup(name);
    shader->location_count++;
    
    return location;
}

GLboolean Shader_set_bool(Shader *const shader, const GLchar *const name, GLboolean value) {
    GLint location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return GL_FALSE;
    }

    glUniform1i(location, (GLint)value);
    
    return GL_TRUE;
}

GLboolean Shader_set_int(Shader *const shader, const GLchar *const name, GLint value) {
    GLint location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return GL_FALSE;
    }
    
    glUniform1i(location, value);
    
    return GL_TRUE;
}

GLboolean Shader_set_float(Shader *const shader, const GLchar *const name, GLfloat value) {
    GLint location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return GL_FALSE;
    }

    glUniform1f(location, value);
    
    return GL_TRUE;
}

GLboolean Shader_set_mat4(Shader *const shader, const GLchar *const name, const GLfloat *value) {
    GLint location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return GL_FALSE;
    }

    glUniformMatrix4fv(location, 1, GL_FALSE, value);

    return GL_TRUE;
}

GLboolean Shader_set_vec3(Shader *const shader, const GLchar *const name, const GLfloat *value) {
    GLint location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return GL_FALSE;
    }

    glUniform3fv(location, 1, value);

    return GL_TRUE; 
}

GLboolean Shader_set_vec4(Shader *const shader, const GLchar *const name, const GLfloat *value) {
    GLint location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return GL_FALSE;
    }

    glUniform4fv(location, 1, value);

    return GL_TRUE; 
}

inline void Shader_set_bool_l(Shader *const shader, const GLint location, GLboolean value) {
    (void)shader;
    glUniform1i(location, (GLint)value);
}

inline void Shader_set_int_l(Shader *const shader, const GLint location, GLint value) {
    (void)shader;
    glUniform1i(location, value);    
}

inline void Shader_set_float_l(Shader *const shader, const GLint location, GLfloat value) {
    (void)shader;    
    glUniform1f(location, value);
}

inline void Shader_set_mat4_l(Shader *const shader, const GLint location, const GLfloat *value) {
    (void)shader;
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

inline void Shader_set_vec3_l(Shader *const shader, const GLint location, const GLfloat *value) {
    (void)shader;
    glUniform3fv(location, 1, value);
}

inline void Shader_set_vec4_l(Shader *const shader, const GLint location, const GLfloat *value) {
    (void)shader;
    glUniform4fv(location, 1, value);
}