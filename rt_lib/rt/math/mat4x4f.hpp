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
};

inline float4 operator*(const mat4x4f& lhs, const float4& rhs)
{
    float4 result;
    for (int row = 0; row < 4; ++row) {
        __m128 acc = _mm_setzero_ps();

        __m128 matRow = lhs.sseData[row];
        acc = _mm_mul_ps(matRow, rhs.sseData);
        acc = _mm_hadd_ps(acc, acc);
        acc = _mm_hadd_ps(acc, acc);

        _mm_store_ss(&result[row], acc);
    }
    return result;
}