//
// Created by Keegan Beaulieu on 2024-07-08.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "List.h"
#include "Mat4x4f.h"
#include "RenderTarget.h"
#include "Texture.h"
#include "Vector3f.h"

#define ISOMETRIC_PITCH 0.615472907

class CCamera : public Component
{
public:
    enum Projection
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    struct Ray
    {
        Vector3f origin;
        Vector3f direction;
    };

    float fov;
    float near;
    float far;
    float scale;
    Projection projection;

    RenderTarget *target;

    Mat4x4f build_vp_matrix();
    Ray ray(Vector2i position);

    static void register_type();
    static void on_add(CCamera *camera);
};

#endif //CAMERA_H
