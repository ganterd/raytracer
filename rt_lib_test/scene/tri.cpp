
#include "gtest/gtest.h"
#include <rt/scene/tri.hpp>

TEST(tri, create)
{
    rt::tri r;
}

TEST(tri, check_simple_intersection)
{
    float4 o(0.0f);
    float4 d(0.0f, 0.0f, 1.0f, 1.0f);
    rt::ray r(o, d);

    float4 v0(-1.0f, -1.0f, 1.0f, 0.0f);
    float4 v1(1.0f, -1.0f, 1.0f, 0.0f);
    float4 v2(0.0f, 1.0f, 1.0f, 0.0f);
    rt::tri t(v0, v2, v1, d, d, d);

    rt::hit h = t.intersect(r);

    EXPECT_TRUE(h.intersected);
    EXPECT_EQ(h.position, float4(0.0f, 0.0f, 1.0f, 0.0f));
    EXPECT_EQ(h.distance, 1.0f);
}
