
#include "gtest/gtest.h"
#include <rt/scene/tri.hpp>

TEST(tri, create)
{
    rt::tri r;
}

/**
 * Tests case where ray is perpendicular, 1 unit away,
 * intersecting through the z-axis.
 */
TEST(tri, check_hit)
{
    float4 o(0.0f);
    float4 d(0.0f, 0.0f, 1.0f, 1.0f);
    rt::ray r(o, d);

    float4 v0(-1.0f, -1.0f, 1.0f, 0.0f);
    float4 v1(1.0f, -1.0f, 1.0f, 0.0f);
    float4 v2(0.0f, 1.0f, 1.0f, 0.0f);
    rt::tri t(v0, v2, v1, d, d, d);

    rt::hit h = t.intersect(r);

    ASSERT_TRUE(h.intersected);
    EXPECT_EQ(h.position, float4(0.0f, 0.0f, 1.0f, 0.0f));
    EXPECT_EQ(h.distance, 1.0f);
}

/**
 * Tests case where ray is perpendicular, 1 unit away,
 * intersecting through the z-axis.
 */
TEST(tri, check_backface_hit)
{
    float4 o(0.0f, 0.0f, 2.0f, 0.0f);
    float4 d(0.0f, 0.0f, -1.0f, 1.0f);
    rt::ray r(o, d);

    float4 v0(-1.0f, -1.0f, 1.0f, 0.0f);
    float4 v1(1.0f, -1.0f, 1.0f, 0.0f);
    float4 v2(0.0f, 1.0f, 1.0f, 0.0f);
    rt::tri t(v0, v2, v1, d, d, d);

    rt::hit h = t.intersect(r);

    ASSERT_TRUE(h.intersected);
    EXPECT_EQ(h.position, float4(0.0f, 0.0f, 1.0f, 0.0f));
    EXPECT_EQ(h.distance, 1.0f);
}

/**
 * Tests case where ray is perpendicular, but misses off to
 * the side of the triangle
 */
TEST(tri, intersection_miss)
{
    float4 o(1.1f, 1.1f, 0.0f, 0.0f);
    float4 d(0.0f, 0.0f, 1.0f, 1.0f);
    rt::ray r(o, d);

    float4 v0(-1.0f, -1.0f, 1.0f, 0.0f);
    float4 v1(1.0f, -1.0f, 1.0f, 0.0f);
    float4 v2(0.0f, 1.0f, 1.0f, 0.0f);
    rt::tri t(v0, v2, v1, d, d, d);

    rt::hit h = t.intersect(r);

    ASSERT_FALSE(h.intersected);
}

/**
 * Tests case where ray is parallel with tri-plane, but raised
 * slightly to miss it.
 */
TEST(tri, intersection_parallel_near_miss)
{
    float4 o(2.0f, 0.0f, 1.0001f, 0.0f);
    float4 d(-1.0f, 0.0f, 0.0f, 1.0f);
    rt::ray r(o, d);

    float4 v0(-1.0f, -1.0f, 1.0f, 0.0f);
    float4 v1(1.0f, -1.0f, 1.0f, 0.0f);
    float4 v2(0.0f, 1.0f, 1.0f, 0.0f);
    rt::tri t(v0, v2, v1, d, d, d);

    rt::hit h = t.intersect(r);

    ASSERT_FALSE(h.intersected);
}

/**
 * Tests case where ray is parallel with tri-plane (shouldn't
 * intersect)
 */
TEST(tri, intersection_same_plane)
{
    float4 o(2.0f, 0.0f, 1.0f, 0.0f);
    float4 d(-1.0f, 0.0f, 0.0f, 1.0f);
    rt::ray r(o, d);

    float4 v0(-1.0f, -1.0f, 1.0f, 0.0f);
    float4 v1(1.0f, -1.0f, 1.0f, 0.0f);
    float4 v2(0.0f, 1.0f, 1.0f, 0.0f);
    rt::tri t(v0, v2, v1, d, d, d);

    rt::hit h = t.intersect(r);

    ASSERT_FALSE(h.intersected);
}