//
// Created by Keegan Beaulieu on 2024-07-08.
//

#include "Component.h"
#include "CCamera.h"

void CCamera::on_add(CCamera *camera)
{
    camera->target = nullptr;
    camera->far = 1000.0f;
    camera->fov = M_PI_2;
    camera->near = 0.1f;
    camera->projection = Projection::ORTHOGRAPHIC;
    camera->scale = 10.0f;
}

Mat4x4f CCamera::build_vp_matrix()
{
    float aspect = (float)target->texture->width/target->texture->height;

    const Vector3f &position = transform->position;
    const Vector3f &rotation = transform->rotation;

    const Mat4x4f translation_matrix = Mat4x4f::translation(-position);
    const Mat4x4f rotation_matrix = Mat4x4f::rotation(-rotation);

    const Mat4x4f view = translation_matrix * rotation_matrix;

    if (projection == Projection::PERSPECTIVE) {
        return view * Mat4x4f::perspective(fov, aspect, near, far);
    } else {
        return Mat4x4f::orthographic(scale, aspect, near, far) * view;
    }
}

CCamera::Ray CCamera::ray(Vector2i position)
{
    float fx = ((float)position.x/target->texture->width - 0.5f)*2.0f;
    float fy = -((float)position.y/target->texture->height - 0.5f)*2.0f;

    Vector4f end4 = build_vp_matrix().inverse()*Vector4f(fx, fy, 0.0f, 1.0f);
    Vector4f start4 = build_vp_matrix().inverse()*Vector4f(fx, fy, -1.0f, 1.0f);
    Vector3f end = Vector3f(end4.x/end4.w, end4.y/end4.w, end4.z/end4.w);
    Vector3f start = Vector3f(start4.x/start4.w, start4.y/start4.w, start4.z/start4.w);
    Vector3f ray = end - start;

    return Ray{ start, ray/ray.length() };
}

void CCamera::register_type()
{
    Component::register_type<CCamera>({
        .on_add = on_add,
        .name = "Camera"
    });
}
