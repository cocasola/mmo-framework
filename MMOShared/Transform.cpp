//
// Created by Keegan Beaulieu on 2024-07-16.
//

#include "Transform.h"

List<Transform> Transform::transforms;

Transform *Transform::create()
{
    Transform *transform = transforms.alloc();
    transform->scale = Vector3f(1, 1, 1);

    return transform;
}

void Transform::destroy(Transform *transform)
{
    transforms.destroy(transform);
}

Mat4x4f Transform::build_trs()
{
    return Mat4x4f::scaling(scale)*Mat4x4f::rotation(rotation)*Mat4x4f::translation(position);
}
