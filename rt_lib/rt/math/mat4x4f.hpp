#pragma once

#include <nmmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>

#include <rt/math/float4.hpp>

class alignas(16) mat4x4f
{
public:
    union
    {
        __m128 sseData[4];
        float data[16];
        float4 rows[4];
    };

    mat4x4f() {};

    static mat4x4f diagonal(float d)
    {
        mat4x4f r;
        r.rows[0] = float4(d, 0, 0, 0);
        r.rows[1] = float4(0, d, 0, 0);
        r.rows[2] = float4(0, 0, d, 0);
        r.rows[3] = float4(0, 0, 0, d);
        return r;
    }

    static mat4x4f identity()
    {
        return diagonal(1.0f);
    };

    static mat4x4f rotation(const float4& eulerAngles)
    {
        const float sz = sinf(eulerAngles.z);
        const float cz = cosf(eulerAngles.z);
        const float sy = sinf(eulerAngles.y);
        const float cy = cosf(eulerAngles.y);
        const float sx = sinf(eulerAngles.x);
        const float cx = cosf(eulerAngles.x);

        mat4x4f r;
        r.rows[0] = float4(cz * cy,   cz * sy * sx - sz * cx,   cz * sy * cx + sz * sx,   0.0f);
        r.rows[1] = float4(sz * cy,   sz * sy * sx + cz * cx,   sz * sy * cx - cz * sx,   0.0f);
        r.rows[2] = float4(-sy,       cy * sx,                  cy * cx,                  0.0f);
        r.rows[3] = float4(0.0f,      0.0f,                     0.0f,                     1.0f);
        return r;
    }

    inline float& operator[](int i)
    {
        return data[i];
    }

    inline const float operator[](int i) const
    {
        return data[i];
    }
};

inline float4 operator*(const mat4x4f& m, const float4& v)
{
    float4 result;
    for (int row = 0; row < 4; ++row) {
        __m128 acc = _mm_setzero_ps();

        __m128 matRow = m.sseData[row];
        acc = _mm_mul_ps(matRow, v.sseData);
        acc = _mm_hadd_ps(acc, acc);
        acc = _mm_hadd_ps(acc, acc);

        _mm_store_ss(&result[row], acc);
    }
    return result;
}

inline mat4x4f inverse(const mat4x4f& m)
{
    mat4x4f inv;
    float det;
    int i;

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
    {
        return m;
    }

    det = 1.0 / det;
    for (i = 0; i < 16; i++)
    {
        inv[i] = inv[i] * det;
    }

    return inv;
}