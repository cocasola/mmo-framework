//
// Created by Keegan Beaulieu on 2024-08-07.
//

#ifndef VECTOR4F_H
#define VECTOR4F_H

struct Vector4f
{
    float x;
    float y;
    float z;
    float w;

    Vector4f() : x(0), y(0), z(0), w(0) { }
    Vector4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
};



#endif //VECTOR4F_H
