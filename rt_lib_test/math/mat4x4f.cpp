
#include "gtest/gtest.h"

#include <rt/math/mat4x4f.hpp>

TEST(mat4x4f, identity)
{
    mat4x4f m = mat4x4f::identity();

    EXPECT_EQ(m.rows[0], float4(1, 0, 0, 0));
    EXPECT_EQ(m.rows[1], float4(0, 1, 0, 0));
    EXPECT_EQ(m.rows[2], float4(0, 0, 1, 0));
    EXPECT_EQ(m.rows[3], float4(0, 0, 0, 1));
}

TEST(mat4x4f, mul_float4_identity)
{
    mat4x4f m = mat4x4f::identity();
    float4 v(1, 2, 3, 4);

    float4 r = m * v;

    EXPECT_EQ(r, v);
}

TEST(mat4x4f, mul_float4_scale)
{
    mat4x4f m = mat4x4f::diagonal(2.0f);
    float4 v(1, 2, 3, 4);

    float4 r = m * v;

    EXPECT_EQ(r, float4(2, 4, 6, 8));
}