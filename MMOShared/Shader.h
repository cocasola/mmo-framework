//
// Created by Keegan Beaulieu on 2024-07-08.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "Colour.h"
#include "List.h"
#include "Mat4x4f.h"
#include "Vector2i.h"

class Shader
{
public:
    struct Defaults
    {
        Shader *diffuse3d;
        Shader *ui_container;
        Shader *ui_text;
        Shader *ui_shadow;
        Shader *ui_shadow_shape;
        Shader *flat;
    };

    enum Type
    {
        SHADER_VERTEX,
        SHADER_FRAGMENT
    };

    std::string name;

    unsigned int program;
    unsigned int vertex;
    unsigned int fragment;

    static Defaults defaults;

    static Shader *create(std::string vertex_path, std::string fragment_path, std::string name = "Shader");
    static Shader *create(unsigned int vertex, unsigned int fragment, std::string name = "Shader");
    static unsigned int compile(std::string path, Type type);
    static void destroy(Shader *shader);

    void bind() const;
    int get_uniform_location(std::string name);

    static void init();
    static void cleanup();

    static void uniform_mat4x4f(int location, const Mat4x4f &value);
    static void uniform1i(int location, int value);
    static void uniform1ui(int location, unsigned int value);
    static void uniform2i(int location, const Vector2i &value);
    static void uniform1f(int location, float value);
    static void uniform3f(int location, const Vector3f &value);
    static void uniform4f(int location, const Colour &value);

private:
    static List<Shader> shaders;

    void cleanup_shader();
};

#endif //SHADER_H
