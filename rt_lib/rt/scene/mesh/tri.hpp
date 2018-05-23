#pragma once
#include <glm/glm.hpp>

#include <rt/scene/ray.hpp>
#include <rt/scene/aabb.hpp>

namespace rt
{
	class Tri
	{
	public:
		glm::vec3 v0, v1, v2;
		glm::vec3 centroid;
		glm::vec3 n0, n1, n2;
		glm::vec3 surfaceNormal;
		glm::vec3 surfacePerpendicular0;
		glm::vec3 surfacePerpendicular1;
		glm::vec3 edge1, edge2;
		glm::vec2 min;
		glm::vec2 max;
		AABB aabb;
		__m128 mSSEOffset;
		__m128 mSSEEdge1, mSSEEdge2;

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
			surfaceNormal = glm::normalize(glm::cross(glm::normalize(edge1), glm::normalize(edge2)));
			surfacePerpendicular0 = glm::normalize(edge1);
			surfacePerpendicular1 = glm::normalize(glm::cross(surfaceNormal, surfacePerpendicular0));

			aabb.mMin = glm::min(glm::min(v0, v1), v2);
			aabb.mMax = glm::max(glm::max(v0, v1), v2);

			centroid = (v0 + v1 + v2) / 3.0f;

			mSSEOffset[0] = -v0.x;
			mSSEOffset[1] = -v0.y;
			mSSEOffset[2] = -v0.z;
			mSSEOffset[3] = 0.0f;

			mSSEEdge1[0] = edge1.x;
			mSSEEdge1[1] = edge1.y;
			mSSEEdge1[2] = edge1.z;
			mSSEEdge1[3] = 0.0f;

			mSSEEdge2[0] = edge2.x;
			mSSEEdge2[1] = edge2.y;
			mSSEEdge2[2] = edge2.z;
			mSSEEdge2[3] = 0.0f;
		}

		inline float DotProduct(__m128 a, __m128 b)
		{
			__m128 r1 = _mm_mul_ps(a, b);
			r1 = _mm_hadd_ps(r1, r1);
			r1 = _mm_hadd_ps(r1, r1);
			return r1[0];
		}

		inline __m128 CrossProduct(__m128 a, __m128 b)
		{
			return _mm_sub_ps(
				_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2))), 
				_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1)))
			);
		}

		bool rayIntersection(const Ray& r, RayHit& out)
		{
			const float e = 0.000001f;
			__m128 tvec, pvec, qvec;
			float det, inv_det;

			pvec = CrossProduct(r.mSSEDirection, mSSEEdge2);
			det = DotProduct(mSSEEdge1, pvec);

			if(det > -e && det < e)
				return false;
			inv_det = 1.0f / det;
			
			tvec = _mm_add_ps(r.mSSEOrigin, mSSEOffset);//v0;
			float u = DotProduct(tvec, pvec) * inv_det;
			if(u < 0.0f || u > 1.0f)
				return false;

			qvec = CrossProduct(tvec, mSSEEdge1);
			float v = DotProduct(r.mSSEDirection, qvec) * inv_det;
			if(v < 0.0f || (u + v > 1.0f))
				return false;

			float d = DotProduct(mSSEEdge2, qvec) * inv_det;
			if(d < e)
				return false;

			out.mDistance = d;
			out.mHitPosition = r.mOrigin + r.mDirection * d;
			out.mSurfaceNormal = surfaceNormal;
			out.mTri = this;
			return true;
			// glm::vec3 tvec, pvec, qvec;
			// float det, inv_det;

			// pvec = glm::cross(r.mDirection, edge2);
			// det = glm::dot(edge1, pvec);

			// if(det > -e && det < e)
			// 	return false;
			// inv_det = 1.0f / det;
			
			// tvec = r.mOrigin - v0;
			// float u = glm::dot(tvec, pvec) * inv_det;
			// if(u < 0.0f || u > 1.0f)
			// 	return false;

			// qvec = glm::cross(tvec, edge1);
			// float v = glm::dot(r.mDirection, qvec) * inv_det;
			// if(v < 0.0f || (u + v > 1.0f))
			// 	return false;

			// float d = glm::dot(edge2, qvec) * inv_det;
			// if(d < e)
			// 	return false;

			// out.mDistance = d;
			// out.mHitPosition = r.mOrigin + r.mDirection * d;
			// out.mSurfaceNormal = surfaceNormal;
			// out.mTri = this;
			// return true;
		}

		glm::vec3 interpolatedNormal(const glm::vec3& p)
		{
			return surfaceNormal;
			
			/* TODO: Can optimize this by precalculations */
			// glm::vec3 v = p - v0;
			// float d00 = glm::dot(edge1, edge1);
			// float d01 = glm::dot(edge1, edge2);
			// float d11 = glm::dot(edge2, edge2);
			// float d20 = glm::dot(v, edge1);
			// float d21 = glm::dot(v, edge2);
			// float denom = d00 * d11 - d01 * d01;

			// float sn0 = (d11 * d20 - d01 * d21) / denom;
			// float sn1 = (d00 * d21 - d01 * d20) / denom;
			// float sn2 = 1.0f - sn0 - sn1;
			
			// return n0 * sn2 + n1 * sn0 + n2 * sn1;
		}

		glm::vec3 localToWorldVector(const glm::vec3& v)
		{
			return glm::vec3(
				v.x * surfacePerpendicular1.x + v.y * surfaceNormal.x + v.z * surfacePerpendicular0.x,
				v.x * surfacePerpendicular1.y + v.y * surfaceNormal.y + v.z * surfacePerpendicular0.y,
				v.x * surfacePerpendicular1.z + v.y * surfaceNormal.z + v.z * surfacePerpendicular0.z
			);
		}
	};
}

