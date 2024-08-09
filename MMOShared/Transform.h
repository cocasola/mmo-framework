//
// Created by Keegan Beaulieu on 2024-07-16.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <List.h>
#include <Mat4x4f.h>

#include "Vector3f.h"

class Entity;

class Transform
{
public:
    Vector3f position;
    Vector3f rotation;
    Vector3f scale;

    Mat4x4f build_trs();

    static Transform *create();
    static void destroy(Transform *transform);

private:
    static List<Transform> transforms;
};

#endif //TRANSFORM_H
