
#include "gtest/gtest.h"

#include <rt/math/float4.hpp>
#include <rt/math/quaternion.hpp>
#include <rt/math/utils.hpp>

TEST(quaternion, vector_rotate_y)
{
    float r = mutils::radians(90.0f);
    quaternion q(r, 0.0f, 1.0f, 0.0f);
    float4 v(1.0f, 0.0f, 0.0f, 0.0f);

    v = q.rotationMatrix() * v;

    float4 expected(0.0f, 0.0f, -1.0f, 0.0f);

    const float smol_number = 0.000001f;
    EXPECT_NEAR(v.x, expected.x, smol_number);
    EXPECT_NEAR(v.y, expected.y, smol_number);
    EXPECT_NEAR(v.z, expected.z, smol_number);
    EXPECT_NEAR(v.w, expected.w, smol_number);
}

/**
 * The direct multiplication of a quat * vector is a wrapper
 * around making a rotation matrix from the quaternion and 
 * multiplying the vector by the matrix. This tests to make
 * sure that works as expected.
 */
TEST(quaternion, rotate_direct_vs_rotationmatrix)
{
    float r = mutils::radians(90.0f);
    quaternion q(r, 0.0f, 1.0f, 0.0f);
    float4 v(1.0f, 0.0f, 0.0f, 0.0f);

    v = q * v;

    float4 expected(0.0f, 0.0f, -1.0f, 0.0f);

    const float smol_number = 0.000001f;
    EXPECT_NEAR(v.x, expected.x, smol_number);
    EXPECT_NEAR(v.y, expected.y, smol_number);
    EXPECT_NEAR(v.z, expected.z, smol_number);
    EXPECT_NEAR(v.w, expected.w, smol_number);
}

TEST(quaternion, vector_rotate_zy)
{
    quaternion qy(mutils::radians(45.0f), 0.0f, 1.0f, 0.0f);
    quaternion qz(mutils::radians(45.0f), 0.0f, 0.0f, 1.0f);
    
    float4 vCombined(1.0f, 0.0f, 0.0f, 0.0f);
    float4 vPiecewise(1.0f, 0.0f, 0.0f, 0.0f);

    vPiecewise = qz * vPiecewise;
    vPiecewise = qy * vPiecewise;

    // First rotate around z, then around y
    vCombined = (qy * qz) * vCombined;

    const float smol_number = 0.000001f;
    EXPECT_NEAR(vPiecewise.x, vCombined.x, smol_number);
    EXPECT_NEAR(vPiecewise.y, vCombined.y, smol_number);
    EXPECT_NEAR(vPiecewise.z, vCombined.z, smol_number);
    EXPECT_NEAR(vPiecewise.w, vCombined.w, smol_number);
}