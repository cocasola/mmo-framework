//
// Created by Keegan Beaulieu on 2024-07-07.
//

#include "Vector3f.h"

#include <iostream>

Vector3f Vector3f::operator*(const float &scalar) const {
    return Vector3f(x * scalar, y * scalar, z * scalar);
}

Vector3f Vector3f::operator/(const float &scalar) const
{
    return Vector3f(x / scalar, y / scalar, z / scalar);
}

Vector3f Vector3f::operator+(const Vector3f &other) const {
    return Vector3f(x + other.x, y + other.y, z + other.z);
}

Vector3f Vector3f::operator-(const Vector3f &other) const
{
    return Vector3f(x - other.x, y - other.y, z - other.z);
}

Vector3f Vector3f::operator-() const
{
    return Vector3f(-x, -y, -z);
}

void Vector3f::print() const {
    std::cout << "[" << x << ", " << y << ", " << z << "]" << std::endl;
}

float Vector3f::length()
{
    return sqrt(x*x + y*y + z*z);
}
