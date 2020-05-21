#pragma once

#include <cmath>
#include <nmmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>

class alignas(16) float4
{
public:
    union
    {
        /* data */
        __m128 sseData;
        float data[4];
        struct 
        {
            float r, g, b, a;
        };
        struct
        {
            float x, y, z, w;
        };
        
    };

    /**
     * 4-element float class
     */
    float4() { }

    /**
     * Single element constructor, equivalant to
     * float4(x, x, x, x)
     */
    float4(float x) : x(x), y(x), z(x), w(x) {}

    /**
     * 4-element float class, created with SSE data
     */
    float4(__m128 sseData) : sseData(sseData) { }

    /**
     * 4-element float class, created with individial elements
     */
    float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

    /**
     * 4-element float class
     */
    inline float& operator[](int i)
    {
        return ((float*)&sseData)[i];
    }

    inline float operator[](int i) const
    {
        return ((float*)&sseData)[i];
    }

    inline void operator+=(const float4& rhs)
    {
        sseData = _mm_add_ps(sseData, rhs.sseData);
    }

    inline void operator-=(const float4& rhs)
    {
        sseData = _mm_sub_ps(sseData, rhs.sseData);
    }

    inline void operator*=(const float4& rhs)
    {
        sseData = _mm_mul_ps(sseData, rhs.sseData);
    }

    inline void operator*=(float rhs)
    {
        sseData = _mm_mul_ps(sseData, _mm_set1_ps(rhs));
    }

    inline void operator/=(const float4& rhs)
    {
        sseData = _mm_div_ps(sseData, rhs.sseData);
    }
};

/**
 * Add two vectors
 */
inline float4 operator+(const float4& lhs, const float4& rhs)
{
    return float4(_mm_add_ps(lhs.sseData, rhs.sseData));
}

inline float4 operator-(const float4& lhs, const float4& rhs)
{
    return float4(_mm_sub_ps(lhs.sseData, rhs.sseData));
}

inline float4 operator*(const float4& lhs, const float4& rhs)
{
    return float4(_mm_mul_ps(lhs.sseData, rhs.sseData));
}

inline float4 operator*(const float4& lhs, const float rhs)
{
    return float4(_mm_mul_ps(lhs.sseData, _mm_set1_ps(rhs)));
}

inline float4 operator/(const float4& lhs, const float4& rhs)
{
    return float4(_mm_div_ps(lhs.sseData, rhs.sseData));
}

inline bool operator==(const float4& lhs, const float4& rhs)
{
    __m128 xorf = _mm_xor_ps(lhs.sseData, rhs.sseData);
    __m128i* xori = (__m128i*)&xorf;
    return _mm_test_all_zeros(*xori, *xori);
}

inline float4 pow(const float4& lhs, float rhs)
{
    return float4(
        powf(lhs.x, rhs),
        powf(lhs.y, rhs),
        powf(lhs.z, rhs),
        powf(lhs.w, rhs)
    );
}

inline float4 clamp(const float4& lhs, float _min, float _max)
{
    return float4(
        std::min(std::max(lhs.x, _min), _max),
        std::min(std::max(lhs.y, _min), _max),
        std::min(std::max(lhs.z, _min), _max),
        std::min(std::max(lhs.w, _min), _max)
    );
}

inline float4 cross(const float4& a, const float4& b) 
{
    __m128 l = _mm_shuffle_ps(a.sseData, a.sseData, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 r = _mm_shuffle_ps(b.sseData, b.sseData, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 lmul = _mm_mul_ps(l, r);
    l = _mm_shuffle_ps(a.sseData, a.sseData, _MM_SHUFFLE(3, 1, 0, 2));
    r = _mm_shuffle_ps(b.sseData, b.sseData, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 rmul = _mm_mul_ps(l, r);
    __m128 cross = _mm_sub_ps(lmul, rmul);
    return float4(cross);
}

inline float dot(const float4& a, const float4& b)
{
    __m128 mulRes, shufReg, sumsReg;
    mulRes = _mm_mul_ps(a.sseData, b.sseData);

    shufReg = _mm_movehdup_ps(mulRes);
    sumsReg = _mm_add_ps(mulRes, shufReg);
    shufReg = _mm_movehl_ps(shufReg, sumsReg);
    sumsReg = _mm_add_ss(sumsReg, shufReg);
    return  _mm_cvtss_f32(sumsReg);
}

inline float length(const float4& a)
{
    __m128 sq = _mm_mul_ps(a.sseData, a.sseData);
    __m128 sq_sum = _mm_hadd_ps(sq, sq);
    float sum = sq_sum[0] + sq_sum[1];
    return sqrtf32(sum);
}

inline float4 normalize(const float4& a)
{
    float4 l(length(a));
    return a / l;
}
