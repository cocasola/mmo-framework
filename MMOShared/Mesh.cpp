//
// Created by Keegan Beaulieu on 2024-07-07.
//

#include "Mesh.h"

#include <GL/glew.h>

#define VBO_VERTICES 0
#define VBO_INDICES 1
#define VBO_UVS 2

#define ARROW_RADIUS 0.05f
#define ARROW_HEAD_LENGTH 0.2f
#define ARROW_HEAD_RADIUS 0.1f
#define ARROW_RES 20

List<Mesh> Mesh::meshes;
Mesh::Defaults Mesh::defaults;

Mesh *Mesh::create(const Params &params)
{
    Mesh *mesh = meshes.alloc();

    mesh->name = params.name;
    mesh->vertices = params.vertices;
    mesh->triangles = params.triangles;
    mesh->uvs = params.uvs;

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glGenBuffers(MESH_VBO_COUNT, mesh->vbos);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[VBO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f)*mesh->vertices.size(), mesh->vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(VBO_VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), nullptr);
    glEnableVertexAttribArray(VBO_VERTICES);

    if (mesh->uvs.size() > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[VBO_UVS]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f)*mesh->uvs.size(), mesh->uvs.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(VBO_UVS, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), nullptr);
        glEnableVertexAttribArray(VBO_UVS);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbos[VBO_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*mesh->triangles.size(), mesh->triangles.data(), GL_STATIC_DRAW);

    return mesh;
}

void Mesh::destroy(Mesh *mesh)
{
    mesh->cleanup_mesh();
    meshes.destroy(mesh);
}

void Mesh::init()
{
    defaults.cube = Mesh::create({
        .name = "Cube",
        .vertices = {
            Vector3f(-1.0f, -1.0f, -1.0f),
            Vector3f( 1.0f, -1.0f, -1.0f),
            Vector3f( 1.0f,  1.0f, -1.0f),
            Vector3f(-1.0f,  1.0f, -1.0f),
            Vector3f(-1.0f, -1.0f,  1.0f),
            Vector3f( 1.0f, -1.0f,  1.0f),
            Vector3f( 1.0f,  1.0f,  1.0f),
            Vector3f(-1.0f,  1.0f,  1.0f)
        },
        .triangles = {
            0, 1, 2, 0, 2, 3,    // Bottom face
            4, 6, 5, 4, 7, 6,    // Top face
            0, 4, 5, 0, 5, 1,    // Front face
            3, 2, 7, 2, 6, 7,    // Back face
            0, 3, 7, 0, 7, 4,    // Left face
            1, 5, 6, 1, 6, 2     // Right face
        }
    });

    defaults.ui_quad = Mesh::create({
        .name = "Ui Quad",
        .vertices = {
            Vector3f(0.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(0.0f, -1.0f, 0.0f),
            Vector3f(1.0f, -1.0f, 0.0f)
        },
        .triangles = {
            0, 2, 3,
            0, 3, 1
        },
        .uvs = {
            Vector2f(0.0f, 1.0f),
            Vector2f(1.0f, 1.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(1.0f, 0.0f)
        }
    });

    std::vector<Vector3f> arrow_verts = { {} };
    std::vector<unsigned int> arrow_tris;

    for (float theta = 0; theta < M_PI*2; theta += M_PI*2/ARROW_RES) {
        arrow_verts.push_back(
            Vector3f(sin(theta)*ARROW_RADIUS, 0.0f, cos(theta)*ARROW_RADIUS)
        );
    }

    for (int i = 1; i < ARROW_RES + 1; ++i) {
        arrow_tris.push_back(0);
        arrow_tris.push_back(i);
        if (i < ARROW_RES)
            arrow_tris.push_back(i + 1);
        else
            arrow_tris.push_back(1);
    }

    for (float theta = 0; theta < M_PI*2; theta += M_PI*2/ARROW_RES) {
        arrow_verts.push_back(
            Vector3f(sin(theta)*ARROW_RADIUS, 1.0f - ARROW_HEAD_LENGTH, cos(theta)*ARROW_RADIUS)
        );
    }

    for (int i = arrow_verts.size() - ARROW_RES; i < arrow_verts.size(); ++i) {
        if (i < arrow_verts.size() - 1) {
            arrow_tris.push_back(i);
            arrow_tris.push_back(i + 1);
            arrow_tris.push_back(i - ARROW_RES);
            arrow_tris.push_back(i - ARROW_RES);
            arrow_tris.push_back(i - ARROW_RES + 1);
            arrow_tris.push_back(i + 1);
        } else {
            arrow_tris.push_back(i);
            arrow_tris.push_back(i - ARROW_RES + 1);
            arrow_tris.push_back(i - ARROW_RES);
            arrow_tris.push_back(i - ARROW_RES);
            arrow_tris.push_back(i - ARROW_RES - ARROW_RES + 1);
            arrow_tris.push_back(i - ARROW_RES + 1);
        }
    }

    arrow_verts.push_back(Vector3f(0.0f, 1.0f, 0.0f));

    for (float theta = 0; theta < M_PI*2; theta += M_PI*2/ARROW_RES) {
        arrow_verts.push_back(
            Vector3f(sin(theta)*ARROW_HEAD_RADIUS, 1.0f - ARROW_HEAD_LENGTH, cos(theta)*ARROW_HEAD_RADIUS)
        );
    }

    for (int i = arrow_verts.size() - ARROW_RES; i < arrow_verts.size(); ++i) {
        arrow_tris.push_back(arrow_verts.size() - ARROW_RES - 1);
        arrow_tris.push_back(i);
        if (i < arrow_verts.size() - 1)
            arrow_tris.push_back(i + 1);
        else
            arrow_tris.push_back(i - ARROW_RES + 1);
    }

    defaults.arrow = Mesh::create({
        .vertices = arrow_verts,
        .triangles = arrow_tris,
        .name = "Arrow"
    });
}

void Mesh::cleanup()
{
    for (auto mesh : meshes) {
        mesh.cleanup_mesh();
    }
}

void Mesh::cleanup_mesh()
{
    glDeleteBuffers(MESH_VBO_COUNT, vbos);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
