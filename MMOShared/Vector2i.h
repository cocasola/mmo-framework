//
// Created by Keegan Beaulieu on 2024-07-09.
//

#ifndef VECTOR2I_H
#define VECTOR2I_H

class Vector2i
{
public:
    int x;
    int y;

    Vector2i() : x(0), y(0) { }
    Vector2i(int x, int y) : x(x), y(y) { }
    Vector2i operator/(const int &scalar) const;
    Vector2i operator*(const int &scalar) const;
    Vector2i operator+(const Vector2i &v) const;
    Vector2i operator-(const Vector2i &v) const;
};

#endif //VECTOR2I_H
