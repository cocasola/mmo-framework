//
// Created by Keegan Beaulieu on 2024-07-08.
//

#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <format>

#include <GL/glew.h>

List<Shader> Shader::shaders;
Shader::Defaults Shader::defaults;

void Shader::cleanup_shader()
{
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteProgram(program);
}

Shader *Shader::create(std::string vertex_path, std::string fragment_path, std::string name)
{
    Shader *shader = shaders.alloc();

    shader->name = name;

    try {
        shader->vertex = compile(vertex_path, SHADER_VERTEX);
        shader->fragment = compile(fragment_path, SHADER_FRAGMENT);

        shader->program = glCreateProgram();
        glAttachShader(shader->program, shader->vertex);
        glAttachShader(shader->program, shader->fragment);
        glLinkProgram(shader->program);

        int success;
        glGetProgramiv(shader->program, GL_LINK_STATUS, &success);

        if(!success) {
            char log[512];
            glGetProgramInfoLog(shader->program, 512, nullptr, log);

            throw std::format("Shader '{}' failed, failed to link: ", name, log);
        }
    } catch (...) {
        if (shader->vertex) glDeleteShader(shader->vertex);
        if (shader->fragment) glDeleteShader(shader->fragment);
        if (shader->program) glDeleteProgram(shader->program);

        throw;
    }

    return shader;
}

Shader *Shader::create(unsigned int vertex, unsigned int fragment, std::string name)
{
    Shader *shader = shaders.alloc();

    shader->name = name;

    try {
        shader->vertex = vertex;
        shader->fragment = fragment;

        shader->program = glCreateProgram();
        glAttachShader(shader->program, shader->vertex);
        glAttachShader(shader->program, shader->fragment);
        glLinkProgram(shader->program);

        int success;
        glGetProgramiv(shader->program, GL_LINK_STATUS, &success);

        if(!success) {
            char log[512];
            glGetProgramInfoLog(shader->program, 512, nullptr, log);

            throw std::format("Shader '{}' failed, failed to link: ", name, log);
        }
    } catch (...) {
        if (shader->program) glDeleteProgram(shader->program);
        throw;
    }

    return shader;
}

unsigned int Shader::compile(std::string path, Type type)
{
    std::fstream file(path);
    if (!file)
        throw std::runtime_error(std::format("Shader failed, failed to open file '{}'.", path));

    std::stringstream string_stream;

    string_stream << file.rdbuf();
    std::string source_string = string_stream.str();
    const char *source = source_string.c_str();

    GLenum gl_type = type == SHADER_FRAGMENT ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER;

    unsigned int shader = glCreateShader(gl_type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        glDeleteShader(shader);

        throw std::runtime_error(std::format("Shader failed, '{}' failed to compile: {}", path, log));
    }

    return shader;
}

void Shader::destroy(Shader *shader)
{
    shader->cleanup_shader();
    shaders.destroy(shader);
}

void Shader::bind() const
{
    glUseProgram(program);
}

int Shader::get_uniform_location(std::string name)
{
    return glGetUniformLocation(program, name.c_str());
}

void Shader::init()
{
    defaults.diffuse3d = create("3d_vertex.glsl", "3d_fragment.glsl", "Diffuse 3D");
    defaults.ui_container = create("uicontainer_vertex.glsl", "uicontainer_fragment.glsl", "UiContainer");
    defaults.ui_shadow = create("uicontainer_vertex.glsl", "uishadow_fragment.glsl", "UiShadow");
    defaults.ui_shadow_shape = create("uicontainer_vertex.glsl", "uishadow_shape_fragment.glsl", "UiShadow Shape");
    defaults.ui_text = create("uitext_vertex.glsl", "uitext_fragment.glsl", "UiText");
    defaults.flat = create("flat_vertex.glsl", "flat_fragment.glsl", "Flat");
}

void Shader::cleanup()
{
    for (auto shader : shaders) {
        shader.cleanup_shader();
    }
}

void Shader::uniform_mat4x4f(int location, const Mat4x4f &value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&value);
}

void Shader::uniform1i(int location, int value)
{
    glUniform1i(location, value);
}

void Shader::uniform1ui(int location, unsigned int value)
{
    glUniform1ui(location, value);
}

void Shader::uniform2i(int location, const Vector2i &value)
{
    glUniform2i(location, value.x, value.y);
}

void Shader::uniform1f(int location, float value)
{
    glUniform1f(location, value);
}

void Shader::uniform3f(int location, const Vector3f &value)
{
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::uniform4f(int location, const Colour &value)
{
    glUniform4f(location, value.r, value.g, value.b, value.a);
}

