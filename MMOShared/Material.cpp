//
// Created by Keegan Beaulieu on 2024-07-19.
//

#include "Material.h"

List<Material> Material::materials;
Material::Defaults Material::defaults;

Material *Material::create(Shader *shader)
{
    Material *material = materials.alloc();
    material->shader = shader;
    material->values = std::unordered_map<std::string, Value>();

    return material;
}

void Material::destroy(Material *material)
{
    material->cleanup_material();
    material->values.~unordered_map();
    materials.destroy(material);
}

void Material::init()
{
    defaults.diffuse3d = create(Shader::defaults.diffuse3d);
    defaults.ui_container = create(Shader::defaults.ui_container);
    defaults.ui_text = create(Shader::defaults.ui_text);
    defaults.ui_shadow = create(Shader::defaults.ui_shadow);
    defaults.ui_shadow_shape = create(Shader::defaults.ui_shadow_shape);
    defaults.flat = create(Shader::defaults.flat);
}

void Material::cleanup()
{
    for (auto material : materials) {
        material.cleanup_material();
    }
}

void Material::cleanup_material()
{
    for (auto i : values) {
        free(i.second.value);
    }
}

void Material::bind()
{
    shader->bind();
    if (texture)
        texture->bind();

    for (auto i : values) {
        const Value &value = i.second;

        switch (value.type) {
            case Value::MAT4X4:
                shader->uniform_mat4x4f(value.location, *(Mat4x4f *)value.value);
                break;
            case Value::INT:
                shader->uniform1i(value.location, *(int *)value.value);
                break;
            case Value::FLOAT:
                shader->uniform1f(value.location, *(float *)value.value);
                break;
            case Value::VEC2I:
                shader->uniform2i(value.location, *(Vector2i *)value.value);
                break;
            case Value::VEC3F:
                shader->uniform3f(value.location, *(Vector3f *)value.value);
                break;
            case Value::COLOUR:
                shader->uniform4f(value.location, *(Colour *)value.value);
                break;
            case Value::UNSIGNED_INT:
                shader->uniform1ui(value.location, *(unsigned int *)value.value);
                break;
        }
    }
}

void Material::set_imp(std::string &name, const void *data, Value::Type type, size_t size)
{
    try {
        Value &value = values.at(name);
        memcpy(value.value, data, size);
    } catch (...) {
        void *copy = malloc(size);
        memcpy(copy, data, size);

        values[name] = {
            shader->get_uniform_location(name),
            copy,
            type
        };
    }
}
template <>
void Material::set<Mat4x4f>(std::string name, const Mat4x4f &mat)
{
    set_imp(name, &mat, Value::MAT4X4, sizeof(Mat4x4f));
}

template <>
void Material::set<Vector3f>(std::string name, const Vector3f &vec)
{
    set_imp(name, &vec, Value::VEC3F, sizeof(Vector3f));
}

template <>
void Material::set<Vector2i>(std::string name, const Vector2i &vec)
{
    set_imp(name, &vec, Value::VEC2I, sizeof(Vector2i));
}

template <>
void Material::set<float>(std::string name, const float &v)
{
    set_imp(name, &v, Value::FLOAT, sizeof(float));
}

template <>
void Material::set<int>(std::string name, const int &v)
{
    set_imp(name, &v, Value::INT, sizeof(int));
}

template <>
void Material::set<Colour>(std::string name, const Colour &v)
{
    set_imp(name, &v, Value::COLOUR, sizeof(Colour));
}

template <>
void Material::set<unsigned int>(std::string name, const unsigned int &v)
{
    set_imp(name, &v, Value::UNSIGNED_INT, sizeof(v));
}
