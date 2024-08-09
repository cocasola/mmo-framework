//
// Created by Keegan Beaulieu on 2024-07-09.
//

#include "Vector2f.h"

#include <iostream>

Vector2f Vector2f::operator*(const float &scalar) const {
    return Vector2f(x * scalar, y * scalar);
}

Vector2f Vector2f::operator+(const Vector2f &other) const {
    return Vector2f(x + other.x, y + other.y);
}

void Vector2f::print() const {
    std::cout << "[" << x << ", " << y << ", " << "]" << std::endl;
}
