
#include "gtest/gtest.h"
#include <rt/scene/ray.hpp>

TEST(ray, create)
{
    rt::ray r;
}

TEST(ray, create_check_inv)
{
    float4 o(0.0f);
    float4 d(1.0f, 0.0f, 0.0f, 1.0f);

    rt::ray r(o, d);

    EXPECT_EQ(r.mOrigin, o);
    EXPECT_EQ(r.mDirection, d);
    EXPECT_EQ(r.mInvDir, float4(1.0f) / d);
}