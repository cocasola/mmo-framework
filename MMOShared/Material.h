//
// Created by Keegan Beaulieu on 2024-07-19.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include <unordered_map>

#include "Shader.h"
#include "Mat4x4f.h"
#include "Texture.h"

class Material
{
public:
    struct Value
    {
        enum Type
        {
            MAT4X4,
            VEC3F,
            VEC2I,
            FLOAT,
            INT,
            COLOUR,
            UNSIGNED_INT
        };

        int location;
        void *value;
        Type type;
    };

    struct Defaults
    {
        Material *diffuse3d;
        Material *ui_container;
        Material *ui_text;
        Material *ui_shadow;
        Material *ui_shadow_shape;
        Material *flat;
    };

    Shader *shader;
    Texture *texture;
    std::unordered_map<std::string, Value> values;

    static Defaults defaults;

    template <typename T>
    void set(std::string name, const T &t);

    void bind();

	static Material *create(Shader *shader);
    static void destroy(Material *material);
    static void init();
    static void cleanup();

private:
	static List<Material> materials;

    void set_imp(std::string &name, const void *data, Value::Type type, size_t size);
    void cleanup_material();
};

#endif //MATERIAL_H
