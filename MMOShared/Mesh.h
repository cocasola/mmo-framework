//
// Created by Keegan Beaulieu on 2024-07-07.
//

#ifndef MESH_H
#define MESH_H

#include <vector>

#include "List.h"
#include "Vector2f.h"
#include "Vector3f.h"

#define MESH_VBO_COUNT 3

class Mesh
{
public:
    struct Params
    {
        std::string name = "Mesh";

        std::vector<Vector3f> vertices;
        std::vector<unsigned int> triangles;
        std::vector<Vector2f> uvs;
    };

    struct Defaults
    {
        Mesh *ui_quad;
        Mesh *cube;
        Mesh *arrow;
    };

    std::string name;

    std::vector<Vector3f> vertices;
    std::vector<unsigned int> triangles;
    std::vector<Vector2f> uvs;

    static Defaults defaults;

    void draw();

    static Mesh *create(const Params &params);
    static void destroy(Mesh *mesh);
    static void init();
    static void cleanup();

private:
    unsigned int vao;
    unsigned int vbos[MESH_VBO_COUNT];

    static List<Mesh> meshes;

    void cleanup_mesh();
};

#endif //MESH_H
