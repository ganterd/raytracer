#pragma once
#include <glm/glm.hpp>

namespace rt
{
	class Tri
	{
	public:
		glm::vec3 v0, v1, v2;
		glm::vec3 n0, n1, n2;
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

			// Precalculate edges
			edge1 = v1 - v0;
			edge2 = v2 - v0;

		}

		bool rayIntersection(
			const glm::vec3& origin,
			const glm::vec3& direction,
			float& outHitDistance,
			glm::vec3& outHit
		){
			const float e = 0.000001f;
			glm::vec3 tvec, pvec, qvec;
			float det, inv_det;

			pvec = glm::cross(direction, edge2);
			det = glm::dot(edge1, pvec);

			if(det > -e && det < e)
				return false;
			inv_det = 1.0f / det;
			
			tvec = origin - v0;
			float u = glm::dot(tvec, pvec) * inv_det;
			if(u < 0.0f || u > 1.0f)
				return false;

			qvec = glm::cross(tvec, edge1);
			float v = glm::dot(direction, qvec) * inv_det;
			if(v < 0.0f || u + v > 1.0f)
				return false;

			outHitDistance = glm::dot(edge2, qvec) * inv_det;
			outHit = origin + direction * outHitDistance;
			return true;
		}
	};
}

