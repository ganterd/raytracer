
#include "gtest/gtest.h"

#include <rt/math/float4.hpp>
#include <rt/math/quaternion.hpp>
#include <rt/math/utils.hpp>

TEST(quaternion, vector_rotate_y)
{
    float r = mutils::radians(90.0f);
    quaternion q(r, 0.0f, 1.0f, 0.0f);
    float4 v(1.0f, 0.0f, 0.0f, 0.0f);

    v = v * q.rotationMatrix();

    float4 expected(0.0f, 0.0f, -1.0f, 0.0f);

    const float smol_number = 0.000001f;
    EXPECT_NEAR(v.x, expected.x, smol_number);
    EXPECT_NEAR(v.y, expected.y, smol_number);
    EXPECT_NEAR(v.z, expected.z, smol_number);
    EXPECT_NEAR(v.w, expected.w, smol_number);
}

TEST(quaternion, vector_rotate_xy)
{
    quaternion qx(mutils::radians(45.0f), 1.0f, 0.0f, 0.0f);
    quaternion qy(mutils::radians(90.0f), 0.0f, 1.0f, 0.0f);
    float4 v(1.0f, 0.0f, 0.0f, 0.0f);

    v = v * (qx * qy).rotationMatrix();

    float4 expected(0.0f, 0.707107f, -0.707107f, 0.0f);

    const float smol_number = 0.000001f;
    EXPECT_NEAR(v.x, expected.x, smol_number);
    EXPECT_NEAR(v.y, expected.y, smol_number);
    EXPECT_NEAR(v.z, expected.z, smol_number);
    EXPECT_NEAR(v.w, expected.w, smol_number);
}