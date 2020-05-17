
#include "gtest/gtest.h"
#include <rt/math/float4.hpp>

/**
 * Assert that a float4 is 4 * 32 bits (important because we're
 * using SSE _mm128 and we don't want the union to be > 128b)
 */
TEST(float4, Footprint)
{
    ASSERT_EQ(sizeof(float4), sizeof(float) * 4);
}

/**
 * Test that a float4 can be assigned through constructor
 */
TEST(float4, Constructor)
{
    float4 a(0.0f, 1.0f, 2.0f, 3.0f);

    for(int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(a[i], (int)i) << "Differed at index " << i;
    }
}

TEST(float4, SSELoad)
{
    __m128 d = _mm_set_ps(4, 3, 2, 1);
    float4 a(d);

    EXPECT_EQ(a.x, 1.0f);
    EXPECT_EQ(a.y, 2.0f);
    EXPECT_EQ(a.z, 3.0f);
    EXPECT_EQ(a.w, 4.0f);
}

/**
 * Test that a float4 can be accessed correctly through rgba
 */
TEST(float4, AccessRGBA)
{
    float4 a(0.0f, 1.0f, 2.0f, 3.0f);

    EXPECT_EQ(a.r, 0.0f);
    EXPECT_EQ(a.g, 1.0f);
    EXPECT_EQ(a.b, 2.0f);
    EXPECT_EQ(a.a, 3.0f);
}

/**
 * Test that a float4 can be updated and values remain
 */
TEST(float4, AccessIndex)
{
    float4 a;
    a[0] = 0.0f;
    a[1] = 1.0f;
    a[2] = 2.0f;
    a[3] = 3.0f;

    for(int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(a[i], (int)i) << "Differed at index " << i;
    }
}

/**
 * Test that two float4 can be added
 */
TEST(float4, Add)
{
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    float4 b(5.0f, 6.0f, 7.0f, 8.0f);
    float4 c = a + b;

    EXPECT_EQ(c.x, 6.0f);
    EXPECT_EQ(c.y, 8.0f);
    EXPECT_EQ(c.z, 10.0f);
    EXPECT_EQ(c.w, 12.0f);
}

/**
 * Test that a float4 can be added to with another float4
 */
TEST(float4, AddUnaryByIndex)
{
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    a[0] += 5.0f;
    a[1] += 6.0f;
    a[2] += 7.0f;
    a[3] += 8.0f;

    EXPECT_EQ(a.x, 6.0f);
    EXPECT_EQ(a.y, 8.0f);
    EXPECT_EQ(a.z, 10.0f);
    EXPECT_EQ(a.w, 12.0f);
}

/**
 * Test that a float4 can be added to with another float4
 */
TEST(float4, AddUnary)
{
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    float4 b(5.0f, 6.0f, 7.0f, 8.0f);
    a += b;

    EXPECT_EQ(a.x, 6.0f);
    EXPECT_EQ(a.y, 8.0f);
    EXPECT_EQ(a.z, 10.0f);
    EXPECT_EQ(a.w, 12.0f);
}

/**
 * Test that two float4 can be subtracted
 */
TEST(float4, Sub)
{
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    float4 b(5.0f, 6.0f, 7.0f, 8.0f);
    float4 c = a - b;

    EXPECT_EQ(c.x, -4.0f);
    EXPECT_EQ(c.y, -4.0f);
    EXPECT_EQ(c.z, -4.0f);
    EXPECT_EQ(c.w, -4.0f);
}

/**
 * Test that two float4 can be subtracted
 */
TEST(float4, SubUnary)
{
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    float4 b(5.0f, 6.0f, 7.0f, 8.0f);
    a -= b;

    EXPECT_EQ(a.x, -4.0f);
    EXPECT_EQ(a.y, -4.0f);
    EXPECT_EQ(a.z, -4.0f);
    EXPECT_EQ(a.w, -4.0f);
}

/**
 * Test that two float4 can be multiplied
 */
TEST(float4, mul_float4_float4)
{
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    float4 b(5.0f, 6.0f, 7.0f, 8.0f);
    float4 c = a * b;

    EXPECT_EQ(c.x, 5.0f);
    EXPECT_EQ(c.y, 12.0f);
    EXPECT_EQ(c.z, 21.0f);
    EXPECT_EQ(c.w, 32.0f);
}

/**
 * Test that two float4 can be multiplied
 */
TEST(float4, mul_float4_float)
{
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    float4 c = a * 2.0f;

    EXPECT_EQ(c.x, 2.0f);
    EXPECT_EQ(c.y, 4.0f);
    EXPECT_EQ(c.z, 6.0f);
    EXPECT_EQ(c.w, 8.0f);
}

/**
 * Test that one float4 can be multiplied by another
 */
TEST(float4, mul_unary_float4)
{
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    float4 b(5.0f, 6.0f, 7.0f, 8.0f);
    a *= b;

    EXPECT_EQ(a.x, 5.0f);
    EXPECT_EQ(a.y, 12.0f);
    EXPECT_EQ(a.z, 21.0f);
    EXPECT_EQ(a.w, 32.0f);
}

/**
 * Test that one float4 can be multiplied by another
 */
TEST(float4, mul_unary_float)
{
    float4 a(1.0f, 2.0f, 3.0f, 4.0f);
    a *= 2.0f;

    EXPECT_EQ(a.x, 2.0f);
    EXPECT_EQ(a.y, 4.0f);
    EXPECT_EQ(a.z, 6.0f);
    EXPECT_EQ(a.w, 8.0f);
}

/**
 * Test that two float4 can be divided
 */
TEST(float4, Div)
{
    float4 a(10.0f, 18.0f, 28.0f, 40.0f);
    float4 b(5.0f, 6.0f, 7.0f, 8.0f);
    float4 c = a / b;

    EXPECT_EQ(c.x, 2.0f);
    EXPECT_EQ(c.y, 3.0f);
    EXPECT_EQ(c.z, 4.0f);
    EXPECT_EQ(c.w, 5.0f);
}

/**
 * Test that one float4 can be divided by another
 */
TEST(float4, DivUnary)
{
    float4 a(10.0f, 18.0f, 28.0f, 40.0f);
    float4 b(5.0f, 6.0f, 7.0f, 8.0f);
    a /= b;

    EXPECT_EQ(a.x, 2.0f);
    EXPECT_EQ(a.y, 3.0f);
    EXPECT_EQ(a.z, 4.0f);
    EXPECT_EQ(a.w, 5.0f);
}

TEST(float4, Power)
{
    float4 a(1, 2, 3, 4);

    a = pow(a, 2.0f);

    EXPECT_EQ(a.x, 1.0f);
    EXPECT_EQ(a.y, 4.0f);
    EXPECT_EQ(a.z, 9.0f);
    EXPECT_EQ(a.w, 16.0f);
}

TEST(float4, Clamp)
{
    float4 a(-1, 0, 0.5, 3);
    a = clamp(a, 0, 1);

    EXPECT_EQ(a.x, 0.0f);
    EXPECT_EQ(a.y, 0.0f);
    EXPECT_EQ(a.z, 0.5f);
    EXPECT_EQ(a.w, 1.0f);
}