#pragma once
#include <glm/glm.hpp>

#include <rt/scene/ray.hpp>
#include <rt/scene/aabb.hpp>

namespace rt
{
	class Tri
	{
	public:
		glm::vec3 v0, n0, n1, n2, edge1, edge2;
		glm::vec3 centroid;
		glm::vec3 surfaceNormal;

		alignas(16) __m128 sn;
		alignas(16) __m128 sp0;
		alignas(16) __m128 sp1;

		AABB aabb;
		__m128 mSSEOrigin;
		__m128 mSSEEdge1, mSSEEdge2;

		Tri()
		{

		}

		Tri(
			const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
			const glm::vec3& n0, const glm::vec3& n1, const glm::vec3& n2
		)
		{
			this->v0 = v0;
			this->n0 = n0;
			this->n1 = n1;
			this->n2 = n2;

			// Precalculate edges
			edge1 = v1 - v0;
			edge2 = v2 - v0;
			surfaceNormal = glm::normalize(glm::cross(glm::normalize(edge1), glm::normalize(edge2)));
			glm::vec3 surfacePerpendicular0 = glm::normalize(edge1);
			glm::vec3 surfacePerpendicular1 = glm::normalize(glm::cross(surfaceNormal, surfacePerpendicular0));

			
			sp1 = _mm_set_ps(0.0f, surfacePerpendicular1.z, surfacePerpendicular1.y, surfacePerpendicular1.x);
			sp0 = _mm_set_ps(0.0f, surfacePerpendicular0.z, surfacePerpendicular0.y, surfacePerpendicular0.x);
			sn = _mm_set_ps(0.0f, surfaceNormal.z, surfaceNormal.y, surfaceNormal.x);

			aabb = rt::AABB::infinity();
			aabb.grow(v0);
			aabb.grow(v1);
			aabb.grow(v2);

			centroid = (v0 + v1 + v2) / 3.0f;

			mSSEOrigin = _mm_set_ps(0.0f, v0.z, v0.y, v0.x);
			mSSEEdge1 = _mm_set_ps(0.0f, edge1.z, edge1.y, edge1.x);
			mSSEEdge2 = _mm_set_ps(0.0f, edge2.z, edge2.y, edge2.x);
		}

		inline float DotProduct(const __m128& a, const __m128& b)
		{
			__m128 r1 = _mm_mul_ps(a, b);
			r1 = _mm_hadd_ps(r1, r1);
			r1 = _mm_hadd_ps(r1, r1);
			return r1[0];
		}

		inline __m128 CrossProduct(const __m128& a, const __m128& b)
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
			
			tvec = _mm_sub_ps(r.mSSEOrigin, mSSEOrigin);//v0;
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

			__m128 SSEd = _mm_set1_ps(d);
			__m128 hit = _mm_fmadd_ps(r.mSSEDirection, SSEd, r.mSSEOrigin);
			out.mHitPosition = glm::vec3(hit[0], hit[1], hit[2]);
			out.mSurfaceNormal = surfaceNormal;
			out.mTri = this;
			return true;
		}

		glm::vec3 interpolatedNormal(const glm::vec3& p)
		{
			//return surfaceNormal;
			
			/* TODO: Can optimize this by precalculations */
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
			
			return glm::normalize(n0 * sn2 + n1 * sn0 + n2 * sn1);
		}

		glm::vec3 localToWorldVector(const glm::vec3& v)
		{
			__m128 vx = _mm_set1_ps(v.x);
			__m128 vy = _mm_set1_ps(v.y);
			__m128 vz = _mm_set1_ps(v.z);

			vx = _mm_mul_ps(vx, sp1);
			vy = _mm_mul_ps(vy, sn);
			vz = _mm_mul_ps(vz, sp0);

			__m128 o = _mm_add_ps(_mm_add_ps(vx, vy), vz);

			return glm::vec3(o[0], o[1], o[2]);
		}
	};
}

