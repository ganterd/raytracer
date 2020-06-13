#pragma once

#include <rt/math/float4.hpp>
#include <rt/math/mat4x4f.hpp>

class quaternion
{
public:
    float w,x,y,z;

    quaternion() { };

    /**
     * @brief Create a quaternion around a certain axis with rotation r in radians
     * @param r Rotation around axis in radians
     * @param axisx x component of rotation axis
     * @param axisy y component of rotation axis
     * @param axisz z component of rotation axis
     */
    quaternion(float r, float axisx, float axisy, float axisz)
    {
        w = cosf(r * 0.5f);
        float4 axis = normalize(float4(axisx, axisy, axisz, 0.0f));
        x = axis.x * sinf(r * 0.5f);
        y = axis.y * sinf(r * 0.5f);
        z = axis.z * sinf(r * 0.5f);
    }

    /**
     * @brief Create the rotation matrix that represents this quaternion
     */
    mat4x4f rotationMatrix() const
    {
        mat4x4f rotationMatrix;
        rotationMatrix.rows[0] = float4(1.0f - 2.0f * y * y - 2.0f * z * z, 2.0f * x * y - 2.0f * w * z, 2.0f * x * z + 2.0f * w * y, 0.0f);
        rotationMatrix.rows[1] = float4(2.0f * x * y + 2.0f * w * z, 1.0f - 2.0f * x * x - 2.0f * z * z, 2.0f * y * z + 2.0f * w * x, 0.0f);
        rotationMatrix.rows[2] = float4(2.0f * x * z - 2.0f * w * y, 2.0f * y * z - 2.0f * w * x, 1.0f - 2.0f * x * x - 2.0f * y * y, 0.0f);
        rotationMatrix.rows[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
        return rotationMatrix;
    }
};

/**
 * @brief Rotate one quaternion by another. 
 * 
 * Rotate one quaternion by another.Ordering of operations is very important here. 
 * The right-hand-side (rhs) is the first rotation applied to a vector. For example:
 * A vector v r
 */
inline quaternion operator*(const quaternion& lhs, const quaternion& rhs)
{
    quaternion q;
    q.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
    q.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
    q.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
    q.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
    return q;
}

/**
 * @brief Rotate a vector by a quaternion.
 */
inline float4 operator*(const quaternion& q, const float4& v)
{
    return q.rotationMatrix() * v;
}