#pragma once
#include <glm/glm.hpp>

#include <rt/scene/ray.hpp>

namespace rt
{
	class Tri
	{
	public:
		glm::vec3 v0, v1, v2;
		glm::vec3 n0, n1, n2;
		glm::vec3 averagedNormal;
		glm::vec3 edge1, edge2;
		glm::vec2 min;
		glm::vec2 max;

		Tri(
			const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
			const glm::vec3& n0, const glm::vec3& n1, const glm::vec3& n2
		)
		{
			this->v0 = v0;
			this->v1 = v1;
			this->v2 = v2;
			this->n0 = n0;
			this->n1 = n1;
			this->n2 = n2;

			averagedNormal = glm::normalize(n0 + n1 + n2);

			// Precalculate edges
			edge1 = v1 - v0;
			edge2 = v2 - v0;

		}

		bool rayIntersection(const Ray& r, RayHit& out)
		{
			const float e = 0.000001f;
			glm::vec3 tvec, pvec, qvec;
			float det, inv_det;

			pvec = glm::cross(r.mDirection, edge2);
			det = glm::dot(edge1, pvec);

			if(det > -e && det < e)
				return false;
			inv_det = 1.0f / det;
			
			tvec = r.mOrigin - v0;
			float u = glm::dot(tvec, pvec) * inv_det;
			if(u < 0.0f || u > 1.0f)
				return false;

			qvec = glm::cross(tvec, edge1);
			float v = glm::dot(r.mDirection, qvec) * inv_det;
			if(v < 0.0f || (u + v > 1.0f))
				return false;

			float d = glm::dot(edge2, qvec) * inv_det;
			if(d <= 0.0f)
				return false;

			out.mDistance = d;
			out.mHitPosition = r.mOrigin + r.mDirection * d;
			out.mSurfaceNormal = averagedNormal;
			out.mTri = this;
			return true;
		}

		glm::vec3 interpolatedNormal(const glm::vec3& p)
		{
			glm::vec3 v = p - v0;
			float d00 = glm::dot(edge1, edge1);
			float d01 = glm::dot(edge1, edge2);
			float d11 = glm::dot(edge2, edge2);
			float d20 = glm::dot(v, edge1);
			float d21 = glm::dot(v, edge2);
			float denom = d00 * d11 - d01 * d01;

			float sn0 = (d11 * d20 - d01 * d21) / denom;
			float sn1 = (d00 * d21 - d01 * d20) / denom;
			float sn2 = 1.0f - sn0 - sn1;
			
			return n0 * sn2 + n1 * sn0 + n2 * sn1;
		}
	};
}

