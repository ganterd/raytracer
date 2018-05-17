#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>

#include <rt/scene/ray.hpp>

namespace rt
{
    class AABB
    {
    public:
        glm::vec3 mMin;
        glm::vec3 mMax;
        glm::vec3 mSize;

        AABB()
        {
            mSize = glm::vec3(0);
        }

        /**
         * Grow to accommodate point p
         */
        void grow(const glm::vec3& p)
        {
            mMin = glm::min(mMin, p);
            mMax = glm::max(mMax, p);
            mSize = mMax - mMin;
        }

        /**
         * Grow to accommodate another AABB
         */
        void grow(const AABB& other)
        {
            mMin = glm::min(mMin, other.mMin);
            mMax = glm::max(mMax, other.mMax);
            mSize = mMax - mMin;
        }

        float surfaceArea()
        {
            return 2.0f * mSize.y * mSize.x + 2.0f * mSize.y * mSize.z + 2.0f * mSize.x * mSize.z;
        }


        static AABB infinity()
        {
            AABB newAABB;
            newAABB.mMin = glm::vec3(+std::numeric_limits<float>::infinity());
            newAABB.mMax = glm::vec3(-std::numeric_limits<float>::infinity());
            newAABB.mSize = glm::vec3(0.0f);
            return newAABB;
        }

        bool intersect(const rt::Ray& r)
        {
            // float tmin, tmax, tymin, tymax, tzmin, tzmax;

            // glm::vec3 signs = glm::step(0.0f, r.mDirection);

            // glm::vec3 farBounds = mMin * (glm::vec3(1.0f) - signs) + mMax * signs;
            // glm::vec3 nearBounds =  mMax * (glm::vec3(1.0f) - signs) + mMin * signs;

            // tmin = (nearBounds.x - r.mOrigin.x) * r.mInverseDirection.x; 
            // tmax = (farBounds.x - r.mOrigin.x) * r.mInverseDirection.x; 
            // tymin = (nearBounds.y - r.mOrigin.y) * r.mInverseDirection.y; 
            // tymax = (farBounds.y - r.mOrigin.y) * r.mInverseDirection.y; 
        
            // if ((tmin > tymax) || (tymin > tmax)) 
            //     return false; 
            // if (tymin > tmin) 
            //     tmin = tymin; 
            // if (tymax < tmax) 
            //     tmax = tymax; 
        
            // tzmin = (nearBounds.z - r.mOrigin.z) * r.mInverseDirection.z; 
            // tzmax = (farBounds.z - r.mOrigin.z) * r.mInverseDirection.z; 
        
            // if ((tmin > tzmax) || (tzmin > tmax)) 
            //     return false; 
            // if (tzmin > tmin) 
            //     tmin = tzmin; 
            // if (tzmax < tmax) 
            //     tmax = tzmax; 
        
            // return true; 
            float tmin = (mMin.x - r.mOrigin.x) / r.mDirection.x; 
            float tmax = (mMax.x - r.mOrigin.x) / r.mDirection.x; 
        
            if (tmin > tmax) std::swap(tmin, tmax); 
        
            float tymin = (mMin.y - r.mOrigin.y) / r.mDirection.y; 
            float tymax = (mMax.y - r.mOrigin.y) / r.mDirection.y; 
        
            if (tymin > tymax) std::swap(tymin, tymax); 
        
            if ((tmin > tymax) || (tymin > tmax)) 
                return false; 
        
            if (tymin > tmin) 
                tmin = tymin; 
        
            if (tymax < tmax) 
                tmax = tymax; 
        
            float tzmin = (mMin.z - r.mOrigin.z) / r.mDirection.z; 
            float tzmax = (mMax.z - r.mOrigin.z) / r.mDirection.z; 
        
            if (tzmin > tzmax) std::swap(tzmin, tzmax); 
        
            if ((tmin > tzmax) || (tzmin > tmax)) 
                return false; 
        
            if (tzmin > tmin) 
                tmin = tzmin; 
        
            if (tzmax < tmax) 
                tmax = tzmax; 
        
            return true; 
        }

        friend std::ostream& operator<< (std::ostream& os, const AABB& a)
        {
            os << "[" << a.mMin.x << "," << a.mMin.y << "," << a.mMin.z << "]->";
            os << "[" << a.mMax.x << "," << a.mMax.y << "," << a.mMax.z << "]";
            return os;
        }
    };
}