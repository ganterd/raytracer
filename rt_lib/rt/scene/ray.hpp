#pragma once

#include <rt/math/float4.hpp>

namespace rt
{
    class ray
    {
    public:
        float4 mDirection;
        float4 mOrigin;
        float4 mInvDir;

        ray() { }     

        ray(const float4& o, const float4& d)
        {
            setOrigin(o);
            setDirection(d);
        }

        ray(const __m128& o, const __m128& d)
        {
            setOrigin(float4(o));
            setDirection(float4(d));
        }

        void setOrigin(const float4& o)
        {
            mOrigin = o;
        }

        void setDirection(const float4& d)
        {

            mDirection = d;
            mInvDir = float4(1.0f) /  mDirection;
        }

        const float4& direction() const
        {
            return mDirection;
        }
    };

    // class Tri;
    // typedef struct _RayHit
    // {
    // public:

    //     float mDistance = std::numeric_limits<float>::max();
    //     glm::vec3 mHitPosition;
    //     glm::vec3 mSurfaceNormal;
    //     glm::vec3 mInterpolatedNormal;
    //     Tri* mTri = nullptr;

    //     int mTrisTested = 0;
    //     int mAABBsTested = 0;
    // } RayHit;
}