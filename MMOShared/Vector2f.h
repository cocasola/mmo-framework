//
// Created by Keegan Beaulieu on 2024-07-09.
//

#ifndef VECTOR2F_H
#define VECTOR2F_H

class Vector2f
{
public:
    float x;
    float y;

    Vector2f() : x(0), y(0) { };
    Vector2f(float x, float y) : x(x), y(y) { };

    Vector2f operator*(const float &scalar) const;
    Vector2f operator+(const Vector2f &other) const;

    void print() const;
};

#endif //VECTOR2F_H
