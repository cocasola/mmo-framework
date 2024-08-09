//
// Created by Keegan Beaulieu on 2024-07-08.
//

#include "Mat4x4f.h"

#include <iostream>
#include <cmath>

Mat4x4f::Mat4x4f()
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

Mat4x4f Mat4x4f::operator*(const Mat4x4f &other) const
{
    Mat4x4f result;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0;

            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }

    return result;
}

Vector3f Mat4x4f::operator*(const Vector3f &vec) const
{
    Vector3f result;

    result.x = m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0];
    result.y = m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1];
    result.z = m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2];

    return result;
}

Vector4f Mat4x4f::operator*(const Vector4f &vec) const
{
    Vector4f result;

    result.x = m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0] * vec.w;
    result.y = m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1] * vec.w;
    result.z = m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2] * vec.w;
    result.w = m[0][3] * vec.x + m[1][3] * vec.y + m[2][3] * vec.z + m[3][3] * vec.w;

    return result;
}

bool Mat4x4f::operator==(const Mat4x4f &other) const
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m[i][j] != other.m[i][j])
                return false;
        }
    }

    return true;
}

Mat4x4f Mat4x4f::inverse()
{
    Mat4x4f result{};
    float inv[16], det;
    float m[16];

    // Convert the 2D array to 1D for simplicity
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            m[i * 4 + j] = this->m[i][j];
        }
    }

    inv[0] = m[5]  * m[10] * m[15] -
             m[5]  * m[11] * m[14] -
             m[9]  * m[6]  * m[15] +
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] -
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
              m[4]  * m[11] * m[14] +
              m[8]  * m[6]  * m[15] -
              m[8]  * m[7]  * m[14] -
              m[12] * m[6]  * m[11] +
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
             m[4]  * m[11] * m[13] -
             m[8]  * m[5] * m[15] +
             m[8]  * m[7] * m[13] +
             m[12] * m[5] * m[11] -
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] -
               m[8]  * m[6] * m[13] -
               m[12] * m[5] * m[10] +
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
              m[1]  * m[11] * m[14] +
              m[9]  * m[2] * m[15] -
              m[9]  * m[3] * m[14] -
              m[13] * m[2] * m[11] +
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
             m[0]  * m[11] * m[14] -
             m[8]  * m[2] * m[15] +
             m[8]  * m[3] * m[14] +
             m[12] * m[2] * m[11] -
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
              m[0]  * m[11] * m[13] +
              m[8]  * m[1] * m[15] -
              m[8]  * m[3] * m[13] -
              m[12] * m[1] * m[11] +
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
              m[0]  * m[10] * m[13] -
              m[8]  * m[1] * m[14] +
              m[8]  * m[2] * m[13] +
              m[12] * m[1] * m[10] -
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
             m[1]  * m[7] * m[14] -
             m[5]  * m[2] * m[15] +
             m[5]  * m[3] * m[14] +
             m[13] * m[2] * m[7] -
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
              m[0]  * m[7] * m[14] +
              m[4]  * m[2] * m[15] -
              m[4]  * m[3] * m[14] -
              m[12] * m[2] * m[7] +
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
              m[0]  * m[7] * m[13] -
              m[4]  * m[1] * m[15] +
              m[4]  * m[3] * m[13] +
              m[12] * m[1] * m[7] -
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
               m[0]  * m[6] * m[13] +
               m[4]  * m[1] * m[14] -
               m[4]  * m[2] * m[13] -
               m[12] * m[1] * m[6] +
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
              m[1] * m[7] * m[10] +
              m[5] * m[2] * m[11] -
              m[5] * m[3] * m[10] -
              m[9] * m[2] * m[7] +
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
             m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11] +
             m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] -
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
               m[0] * m[7] * m[9] +
               m[4] * m[1] * m[11] -
               m[4] * m[3] * m[9] -
               m[8] * m[1] * m[7] +
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
              m[0] * m[6] * m[9] -
              m[4] * m[1] * m[10] +
              m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return Mat4x4f();

    det = 1.0 / det;

    for (int i = 0; i < 16; i++)
        inv[i] = inv[i] * det;

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result.m[i][j] = inv[i * 4 + j];
        }
    }

    return result;
}

Mat4x4f Mat4x4f::translation(const Vector3f &v)
{
    Mat4x4f result{};

    result.m[3][0] = v.x;
    result.m[3][1] = v.y;
    result.m[3][2] = v.z;

    return result;
}

Mat4x4f Mat4x4f::rotation(const Vector3f &v)
{
    Mat4x4f rx{}, ry{}, rz{};

    rx.m[1][1] = cos(v.x);
    rx.m[1][2] = -sin(v.x);
    rx.m[2][1] = sin(v.x);
    rx.m[2][2] = cos(v.x);

    ry.m[0][0] = cos(v.y);
    ry.m[0][2] = sin(v.y);
    ry.m[2][0] = -sin(v.y);
    ry.m[2][2] = cos(v.y);

    rz.m[0][0] = cos(v.z);
    rz.m[0][1] = -sin(v.z);
    rz.m[1][0] = sin(v.z);
    rz.m[1][1] = cos(v.z);

    return rz * ry * rx;
}

Mat4x4f Mat4x4f::scaling(const Vector3f &v)
{
    Mat4x4f result{};

    result.m[0][0] = v.x;
    result.m[1][1] = v.y;
    result.m[2][2] = v.z;

    return result;
}

Mat4x4f Mat4x4f::perspective(float fov, float aspect, float near, float far)
{
    Mat4x4f result{};

    float tan_half_fov = std::tan(fov / 2.0f);

    result.m[0][0] = 1.0f / (aspect * tan_half_fov);
    result.m[1][1] = 1.0f / tan_half_fov;
    result.m[2][2] = -(far + near) / (far - near);
    result.m[2][3] = -1.0f;
    result.m[3][2] = -(2.0f * far * near) / (far - near);

    return result;
}

Mat4x4f Mat4x4f::orthographic(float scale, float aspect, float near, float far)
{
    Mat4x4f result{};

    float right = scale * aspect;
    float left = -right;
    float top = scale;
    float bottom = -top;

    result.m[0][0] = 2.0f / (right - left);
    result.m[1][1] = 2.0f / (top - bottom);
    result.m[2][2] = -2.0f / (far - near);
    result.m[0][3] = -(right + left) / (right - left);
    result.m[1][3] = -(top + bottom) / (top - bottom);
    result.m[2][3] = -(far + near) / (far - near);
    result.m[3][3] = 1.0f;

    return result;
}

void Mat4x4f::print()
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << m[i][j] << " ";
        }

        std::cout << std::endl;
    }
}
