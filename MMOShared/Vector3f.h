//
// Created by Keegan Beaulieu on 2024-07-07.
//

#ifndef VECTOR3F_H
#define VECTOR3F_H

#include "Serialization.h"

struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f() : x(0), y(0), z(0) { };
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) { };

    Vector3f operator*(const float &scalar) const;
    Vector3f operator/(const float &scalar) const;
    Vector3f operator+(const Vector3f &other) const;
    Vector3f operator-(const Vector3f &other) const;
    Vector3f operator-() const;

    void print() const;
    float length();

    SERIALIZABLE({
        PRIMITIVE(Vector3f, x)
        PRIMITIVE(Vector3f, y)
        PRIMITIVE(Vector3f, z)
    })
};

#endif //VECTOR3F_H
