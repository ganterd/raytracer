
#include "gtest/gtest.h"

#include <rt/scene/aabb.hpp>

TEST(aabb, create_infinity)
{
    rt::aabb bb = rt::aabb::infinity();

    float4 expectedMin(+std::numeric_limits<float>::infinity());
    float4 expectedMax(-std::numeric_limits<float>::infinity());
    float4 expectedSize(0.0f);

    EXPECT_EQ(bb.bmin, expectedMin);
    EXPECT_EQ(bb.bmax, expectedMax);
    EXPECT_EQ(bb.bsize, expectedSize);
}

TEST(aabb, add_single_point)
{
    rt::aabb bb = rt::aabb::infinity();

    float4 p(1, 1, 1, 0);
    bb.grow(p);

    float4 expectedSize(0.0f);

    EXPECT_EQ(bb.bmin, p);
    EXPECT_EQ(bb.bmax, p);
    EXPECT_EQ(bb.bsize, expectedSize);
}

TEST(aabb, add_two_points)
{
    rt::aabb bb = rt::aabb::infinity();
    float4 p1(1, 1, 1, 0);
    float4 p2(-1, -1, -1, 0);

    bb.grow(p1);
    bb.grow(p2);
    
    float4 expectedSize(2, 2, 2, 0);
    EXPECT_EQ(bb.bmin, p2);
    EXPECT_EQ(bb.bmax, p1);
    EXPECT_EQ(bb.bsize, expectedSize);
}

/**
 * Surface area of a 2x2x2 box should be 4 + 4 + 4 + 4 + 4 + 4 = 24
 */
TEST(aabb, surface_area)
{
    rt::aabb bb = rt::aabb::infinity();
    float4 p1(1, 1, 1, 0);
    float4 p2(-1, -1, -1, 0);

    bb.grow(p1);
    bb.grow(p2);
    
    float4 expectedSize(2, 2, 2, 0);
    float expectedSurfaceArea = 24.0f;

    float actualSurfaceArea = bb.surfaceArea();

    EXPECT_EQ(bb.bmin, p2);
    EXPECT_EQ(bb.bmax, p1);
    EXPECT_EQ(bb.bsize, expectedSize);
    EXPECT_EQ(actualSurfaceArea, expectedSurfaceArea);
}

TEST(aabb, grow_aabb)
{
    rt::aabb bb_top = rt::aabb::infinity();
    rt::aabb bb_child_left = rt::aabb::infinity();
    rt::aabb bb_child_right = rt::aabb::infinity();

    float4 child_left_p1(1, 1, 1, 0);
    float4 child_left_p2(0.5, 0.5, 0.5, 0);
    bb_child_left.grow(child_left_p1);
    bb_child_left.grow(child_left_p2);

    float4 child_right_p1(-1, -1, -1, 0);
    float4 child_right_p2(-0.5f, -0.5f, -0.5f, 0);
    bb_child_right.grow(child_right_p1);
    bb_child_right.grow(child_right_p2);

    bb_top.grow(bb_child_left);
    bb_top.grow(bb_child_right);
    
    float4 expectedSize(2, 2, 2, 0);
    EXPECT_EQ(bb_top.bmin, child_right_p1);
    EXPECT_EQ(bb_top.bmax, child_left_p1);
    EXPECT_EQ(bb_top.bsize, expectedSize);
}

TEST(aabb, intersect_outside)
{
    rt::aabb bb = rt::aabb::infinity();
    float4 p1(1, 1, 1, 0);
    float4 p2(-1, -1, -1, 0);
    bb.grow(p1);
    bb.grow(p2);

    rt::ray r(
        float4(0, 0, -2, 0),
        float4(0, 0, 1, 1)
    );

    EXPECT_TRUE(bb.intersect(r).intersected);
}

TEST(aabb, intersect_inside)
{
    rt::aabb bb = rt::aabb::infinity();
    float4 p1(1, 1, 1, 0);
    float4 p2(-1, -1, -1, 0);
    bb.grow(p1);
    bb.grow(p2);

    rt::ray r(
        float4(0, 0, 0, 0),
        float4(0, 0, 1, 1)
    );

    EXPECT_TRUE(bb.intersect(r).intersected);
}

TEST(aabb, intersect_miss)
{
    rt::aabb bb = rt::aabb::infinity();
    float4 p1(1, 1, 1, 0);
    float4 p2(-1, -1, -1, 0);
    bb.grow(p1);
    bb.grow(p2);

    rt::ray r(
        float4(4, 0, -2, 0),
        float4(0, 0, 1, 1)
    );

    EXPECT_FALSE(bb.intersect(r).intersected);
}