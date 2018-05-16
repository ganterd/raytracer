#pragma once

#include <glm/glm.hpp>
#include <iostream>

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

        friend std::ostream& operator<< (std::ostream& os, const AABB& a)
        {
            os << "[" << a.mMin.x << "," << a.mMin.y << "," << a.mMin.z << "]->";
            os << "[" << a.mMax.x << "," << a.mMax.y << "," << a.mMax.z << "]";
            return os;
        }
    };
}