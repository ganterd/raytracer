
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

    float4 r = v * m;

    EXPECT_EQ(r, v);
}

TEST(mat4x4f, mul_float4_scale)
{
    mat4x4f m = mat4x4f::diagonal(2.0f);
    float4 v(1, 2, 3, 4);

    float4 r = v * m;

    EXPECT_EQ(r, float4(2, 4, 6, 8));
}

/**
 * Expect a 0-degree rotation to leave a vector unchanged
 */
TEST(mat4x4f, rotate_none)
{
    float toRadians = M_PI / 180.0f;
    mat4x4f m = mat4x4f::rotation(float4(0.0f, 0.0f, 0.0f, 0.0f));
    float4 v(0.0f, 0.0f, 1.0f, 0.0f);

    v = v * m;

    float4 expected(0.0f, 0.0f, 1.0f, 0.0f);

    // This is where FP32 precision issues might start happenning
    EXPECT_EQ(v, expected);
}

/**
 * Expect a 90 degree rotation around the x-axis.
 */
TEST(mat4x4f, create_rotation_x)
{
    float r = 90.0f * (M_PI / 180.0f);
    mat4x4f m = mat4x4f::rotation(float4(r, 0.0f, 0.0f, 0.0f));

    mat4x4f expected;
    expected.rows[0] = float4(1.0f, 0.0f, 0.0f, 0.0f);
    expected.rows[1] = float4(0.0f, cosf(r), -sinf(r), 0.0f);
    expected.rows[2] = float4(0.0f, sinf(r),  cosf(r), 0.0f);
    expected.rows[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);

    for(int i = 0; i < 16; ++i) EXPECT_FLOAT_EQ(m.data[i], expected.data[i]);
}

/**
 * Expect a 90 degree rotation around the y-axis.
 */
TEST(mat4x4f, create_rotation_y)
{
    float r = 90.0f * (M_PI / 180.0f);
    mat4x4f m = mat4x4f::rotation(float4(0.0f, r, 0.0f, 0.0f));

    mat4x4f expected;
    expected.rows[0] = float4(cosf(r), 0.0f, sinf(r), 0.0f);
    expected.rows[1] = float4(0.0f, 1.0f, 0.0f, 0.0f);
    expected.rows[2] = float4(-sinf(r), 0.0f,  cosf(r), 0.0f);
    expected.rows[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);

    for(int i = 0; i < 16; ++i) EXPECT_FLOAT_EQ(m.data[i], expected.data[i]);
}

/**
 * Expect a 90 degree rotation around the y-axis.
 */
TEST(mat4x4f, create_rotation_z)
{
    float r = 90.0f * (M_PI / 180.0f);
    mat4x4f m = mat4x4f::rotation(float4(0.0f, 0.0f, r, 0.0f));

    mat4x4f expected;
    expected.rows[0] = float4(cosf(r), -sinf(r), 0.0f, 0.0f);
    expected.rows[1] = float4(sinf(r), cosf(r), 0.0f, 0.0f);
    expected.rows[2] = float4(0.0f, 0.0f,  1.0f, 0.0f);
    expected.rows[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);

    for(int i = 0; i < 16; ++i) EXPECT_FLOAT_EQ(m.data[i], expected.data[i]);
}

/**
 * Expect a 90 degree rotation around the x-axis.
 */
TEST(mat4x4f, rotate_x)
{
    float r = 90.0f * (M_PI / 180.0f);
    mat4x4f m = mat4x4f::rotation(float4(r, 0.0f, 0.0f, 0.0f));
    float4 v(0.0f, 0.0f, 1.0f, 0.0f);

    v = v * m;

    float4 expected(0.0f, -1.0f, 0.0f, 0.0f);

    // This is where FP32 precision issues might start happenning
    const float smol_number = 0.0000001f;
    EXPECT_NEAR(v.x, expected.x, smol_number);
    EXPECT_NEAR(v.y, expected.y, smol_number);
    EXPECT_NEAR(v.z, expected.z, smol_number);
    EXPECT_NEAR(v.w, expected.w, smol_number);
}

/**
 * Expect a 90 degree rotation around the y-axis.
 */
TEST(mat4x4f, rotate_y)
{
    float r = 90.0f * (M_PI / 180.0f);
    mat4x4f m = mat4x4f::rotation(float4(0.0f, r, 0.0f, 0.0f));
    float4 v(0.0f, 0.0f, 1.0f, 0.0f);

    v = v * m;

    float4 expected(1.0f, 0.0f, 0.0f, 0.0f);

    // This is where FP32 precision issues might start happenning
    const float smol_number = 0.0000001f;
    EXPECT_NEAR(v.x, expected.x, smol_number);
    EXPECT_NEAR(v.y, expected.y, smol_number);
    EXPECT_NEAR(v.z, expected.z, smol_number);
    EXPECT_NEAR(v.w, expected.w, smol_number);
}

/**
 * Expect a 90 degree rotation around the z-axis.
 */
TEST(mat4x4f, rotate_z)
{
    float r = 90.0f * (M_PI / 180.0f);
    mat4x4f m = mat4x4f::rotation(float4(0.0f, 0.0f, r, 0.0f));
    float4 v(0.0f, 1.0f, 0.0f, 0.0f);

    v = v * m;

    float4 expected(-1.0f, 0.0f, 0.0f, 0.0f);

    // This is where FP32 precision issues might start happenning
    const float smol_number = 0.0000001f;
    EXPECT_NEAR(v.x, expected.x, smol_number);
    EXPECT_NEAR(v.y, expected.y, smol_number);
    EXPECT_NEAR(v.z, expected.z, smol_number);
    EXPECT_NEAR(v.w, expected.w, smol_number);
}

TEST(mat4x4f, rotate_xy)
{
    float rx = 45.0f * (M_PI / 180.0f);
    float ry = 90.0f * (M_PI / 180.0f);
    mat4x4f mx = mat4x4f::rotation(float4(rx, 0.0f, 0.0f, 0.0f));
    mat4x4f my = mat4x4f::rotation(float4(0.0f, ry, 0.0f, 0.0f));
    float4 v(1.0f, 0.0f, 0.0f, 0.0f);

    v = v * my * mx;

    float4 expected(0.0f, 0.707107f, -0.707107f, 0.0f);

    // This is where FP32 precision issues might start happenning
    const float smol_number = 0.000001f;
    EXPECT_NEAR(v.x, expected.x, smol_number);
    EXPECT_NEAR(v.y, expected.y, smol_number);
    EXPECT_NEAR(v.z, expected.z, smol_number);
    EXPECT_NEAR(v.w, expected.w, smol_number);
}

TEST(mat4x4f, rotate_rotate_inverse)
{
    float toRadians = M_PI / 180.0f;
    float angle = 90.0f * toRadians;
    mat4x4f m = mat4x4f::rotation(float4(0.0f, 0.0f, angle, 0.0f));
    mat4x4f m_inv = inverse(m);

    float4 v(0.0f, 1.0f, 0.0f, 0.0f);

    float4 actual = v * m;
    actual = actual * m_inv;

    // This is where FP32 precision issues might start happenning
    EXPECT_EQ(actual, v);
}