#pragma once

#include <iostream>
#include <algorithm>

#include <rt/math/float4.hpp>
#include <rt/scene/ray.hpp>

namespace rt
{
    class alignas(alignof(float4)) aabb
    {
    public:
        float4 bmin;
        float4 bmax;
        float4 bsize;

        aabb()
        {
            bmin = float4(0.0f);
            bmax = float4(0.0f);
            bsize = float4(0.0f);
        }

        /**
         * Grow to accommodate point p
         */
        void grow(const float4& p)
        {
            bmin = min(bmin, p);
            bmax = max(bmax, p);
            bsize = bmax - bmin;
        }

        /**
         * Grow to accommodate another AABB
         */
        void grow(const aabb& other)
        {
            bmin = min(bmin, other.bmin);
            bmax = max(bmax, other.bmax);
            bsize = bmax - bmin;
        }

        float surfaceArea()
        {
            return 2.0f * bsize.y * bsize.x + 2.0f * bsize.y * bsize.z + 2.0f * bsize.x * bsize.z;
        }

        static aabb infinity()
        {
            aabb newAABB;
            newAABB.bmin = float4(+std::numeric_limits<float>::infinity());
            newAABB.bmax = float4(-std::numeric_limits<float>::infinity());
            newAABB.bsize = float4(0.0f);
            return newAABB;
        }

        bool intersect(const rt::ray& r) const
        {
            float4 _tmin = (bmin - r.mOrigin) * r.mInvDir;
            float4 _tmax = (bmax - r.mOrigin) * r.mInvDir;
            float4 _min = min(_tmin, _tmax);
            float4 _max = max(_tmin, _tmax);

            float tmin = _min[0];
            tmin = std::max(tmin, _min[1]);
            tmin = std::max(tmin, _min[2]);

            float tmax = _max[0];
            tmax = std::min(tmax, _max[1]);
            tmax = std::min(tmax, _max[2]);
        
            return tmax >= tmin && tmax > 0;
        }

        friend std::ostream& operator<< (std::ostream& os, const aabb& a)
        {
            os << "[" << a.bmin[0] << "," << a.bmin[1] << "," << a.bmin[2] << "]->";
            os << "[" << a.bmax[0] << "," << a.bmax[1] << "," << a.bmax[2] << "]";
            return os;
        }
    };
}