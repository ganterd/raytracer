#pragma once
#include <glm/glm.hpp>

#include <rt/scene/ray.hpp>
#include <rt/scene/aabb.hpp>

namespace rt
{
	class alignas(alignof(__m128)) Tri
	{
	public:
		alignas(alignof(__m128)) __m128 sn;
		alignas(alignof(__m128)) __m128 sp0;
		alignas(alignof(__m128)) __m128 sp1;

		alignas(alignof(__m128)) __m128 tx;
		alignas(alignof(__m128)) __m128 ty;
		alignas(alignof(__m128)) __m128 tz;

		
		alignas(alignof(__m128)) __m128 mSSEOrigin;
		alignas(alignof(__m128)) __m128 mSSEEdge1;
		alignas(alignof(__m128)) __m128 mSSEEdge2;

		glm::vec3 v0, n0, n1, n2, edge1, edge2;
		glm::vec3 centroid;
		glm::vec3 surfaceNormal;
		AABB aabb;

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

			tx = _mm_set_ps(0.0f, surfacePerpendicular0.x, surfaceNormal.x, surfacePerpendicular1.x);
			ty = _mm_set_ps(0.0f, surfacePerpendicular0.y, surfaceNormal.y, surfacePerpendicular1.y);
			tz = _mm_set_ps(0.0f, surfacePerpendicular0.z, surfaceNormal.z, surfacePerpendicular1.z);
		}

		inline __m128 CrossProduct(const __m128& a, const __m128& b) 
		{
			alignas(alignof(__m128)) __m128 l = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
			alignas(alignof(__m128)) __m128 r = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));
			alignas(alignof(__m128)) __m128 lmul = _mm_mul_ps(l, r);
			l = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2));
			r = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
			alignas(alignof(__m128)) __m128 rmul = _mm_mul_ps(l, r);
			alignas(alignof(__m128)) __m128 cross = _mm_sub_ps(lmul, rmul);
			return cross;
		}

		// bool Intersect(const Ray &r, const PrecomputedTriangle &p, Hit &h)
		// {
		// 	const float arr [4] = {−1,−1,−1,1};
		// 	const __m128 int_coef = _mm_load_ps(arr);

		// 	const __m128 o = r->mSSE;
		// 	const __m128 d = _mm_load_ps(&r.dx);
		// 	const __m128 n = _mm_load_ps(&p.nx);
		// 	const __m128 det = _mm_dp_ps(n, d, 0x7f);
		// 	const __m128 dett = _mm_dp_ps(_mm_mul_ps(int_coef, n), o, 0xff);
		// 	const __m128 oldt = _mm_load_ss(&h.t);

		// 	if((_mm_movemask_ps(_mm_xor_ps(dett, _mm_sub_ss(_mm_mul_ss(oldt, det), dett)))&1) == 0)
		// 	{

		// 		const __m128 detp = _mm_add_ps(_mm_mul_ps(o, det),	_mm_mul_ps(dett, d));
		// 		const __m128 detu = _mm_dp_ps(detp,	_mm_load_ps(&p.ux), 0xf1);

		// 		if((_mm_movemask_ps(_mm_xor_ps(detu, _mm_sub_ss(det, detu)))&1) == 0)
		// 		{

		// 			const __m128 detv = _mm_dp_ps(detp,	_mm_load_ps(&p.vx), 0xf1));
		// 			if((_mm_movemask_ps(_mm_xor_ps(detv, _mm_sub_ss(det, _mm_add_ss(detu, detv))))&1) == 0)
		// 			{

		// 				const __m128 inv_det = inv_ss(det);
		// 				_mm_store_ss(&h.t, _mm_mul_ss(dett, inv_det));
		// 				_mm_store_ss(&h.u, _mm_mul_ss(detu, inv_det));
		// 				_mm_store_ss(&h.v, _mm_mul_ss(detv, inv_det));
		// 				_mm_store_ps(&h.px, _mm_mul_ps(detp,
		// 				_mm_shuffle_ps(inv_det, inv_det, 0)));

		// 				return true;

		// 			}
		// 		}
		// 	}

		// 	return false;
		// }

		bool rayIntersection(const rt::Ray* r, RayHit& out)
		{
			const float e = 0.000001f;
			alignas(16) const __m128 tvec = _mm_sub_ps(r->mSSEOrigin, mSSEOrigin);;
			alignas(16) const __m128 pvec = CrossProduct(r->mSSEDirection, mSSEEdge2); 
			alignas(16) const __m128 qvec = CrossProduct(tvec, mSSEEdge1);

			float det, inv_det;

			det = _mm_dp_ps(mSSEEdge1, pvec, 0x7f)[0];

			if(det > -e && det < e)
				return false;
			inv_det = 1.0f / det;
			
			float u = _mm_dp_ps(tvec, pvec, 0x7f)[0] * inv_det;
			if(u < 0.0f || u > 1.0f)
				return false;

			float v = _mm_dp_ps(r->mSSEDirection, qvec, 0x7f)[0] * inv_det;
			if(v < 0.0f || (u + v > 1.0f))
				return false;

			float d = _mm_dp_ps(mSSEEdge2, qvec, 0x7f)[0] * inv_det;
			if(d < e)
				return false;

			out.mDistance = d;

			__m128 SSEd = _mm_set1_ps(d);
			__m128 hit = _mm_fmadd_ps(r->mSSEDirection, SSEd, r->mSSEOrigin);
			out.mHitPosition = glm::vec3(hit[0], hit[1], hit[2]);
			out.mSurfaceNormal = surfaceNormal;
			out.mTri = this;
			return true;
		}

		glm::vec3 interpolatedNormal(const glm::vec3&)
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
			
			// return glm::normalize(n0 * sn2 + n1 * sn0 + n2 * sn1);
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

		__m128 localToWorldVector(const __m128& v)
		{
			__m128 vx = _mm_mul_ps(v, tx);
			__m128 vy = _mm_mul_ps(v, ty);
			__m128 vz = _mm_mul_ps(v, tz);

			__m128 o;
			o[0] = vx[0] + vx[1] + vx[2];
			o[1] = vy[0] + vy[1] + vy[2];
			o[2] = vz[0] + vz[1] + vz[2];
			o[3] = 0.0f;

			return o;
		}
	};
}

