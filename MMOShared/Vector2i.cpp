//
// Created by Keegan Beaulieu on 2024-07-09.
//

#include "Vector2i.h"

Vector2i Vector2i::operator/(const int &scalar) const
{
    return Vector2i(x / scalar, y / scalar);
}

Vector2i Vector2i::operator*(const int &scalar) const
{
    return Vector2i(x * scalar, y * scalar);
}

Vector2i Vector2i::operator+(const Vector2i &v) const
{
    return Vector2i(x + v.x, y + v.y);
}

Vector2i Vector2i::operator-(const Vector2i &v) const
{
    return Vector2i(x - v.x, y - v.y);
}
