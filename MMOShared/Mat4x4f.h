//
// Created by Keegan Beaulieu on 2024-07-08.
//

#ifndef MAT4X4F_H
#define MAT4X4F_H

#include <iostream>
#include <cmath>

#include "Vector3f.h"
#include "Vector4f.h"

struct Mat4x4f {
    float m[4][4];

    Mat4x4f();

    Mat4x4f operator*(const Mat4x4f &other) const;
    Vector3f operator*(const Vector3f &vec) const;
    Vector4f operator*(const Vector4f &vec) const;
    bool operator==(const Mat4x4f &other) const;

    Mat4x4f inverse();
    void print();

    static Mat4x4f translation(const Vector3f &v);
    static Mat4x4f rotation(const Vector3f &v);
    static Mat4x4f scaling(const Vector3f &v);
    static Mat4x4f perspective(float fov, float aspect, float near, float far);
    static Mat4x4f orthographic(float scale, float aspect, float near, float far);
};

#endif //MAT4X4F_H
