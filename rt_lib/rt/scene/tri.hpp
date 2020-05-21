#pragma once

#include <rt/scene/ray.hpp>
#include <rt/scene/rayhit.hpp>

namespace rt
{
	class alignas(float4) tri
	{
	public:
		float4 sn;
		float4 sp0;
		float4 sp1;

		float4 tx;
		float4 ty;
		float4 tz;

		float4 v0, n0, n1, n2, edge1, edge2;
		float4 centroid;
		float4 surfaceNormal;
		//AABB aabb;

		tri(){ }

		tri(
			const float4& v0, 
			const float4& v1,
			const float4& v2,
			const float4& n0,
			const float4& n1,
			const float4& n2
		) {
			this->v0 = v0;
			this->n0 = n0;
			this->n1 = n1;
			this->n2 = n2;

			// Precalculate edges
			edge1 = v1 - v0;
			edge2 = v2 - v0;
			surfaceNormal = normalize(cross(normalize(edge1), normalize(edge2)));
			float4 surfacePerpendicular0 = normalize(edge1);
			float4 surfacePerpendicular1 = normalize(cross(surfaceNormal, surfacePerpendicular0));

			centroid = (v0 + v1 + v2) / 3.0f;

			tx = _mm_set_ps(0.0f, surfacePerpendicular0.x, surfaceNormal.x, surfacePerpendicular1.x);
			ty = _mm_set_ps(0.0f, surfacePerpendicular0.y, surfaceNormal.y, surfacePerpendicular1.y);
			tz = _mm_set_ps(0.0f, surfacePerpendicular0.z, surfaceNormal.z, surfacePerpendicular1.z);
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

		rt::hit intersect(const rt::ray& r)
		{
			const float e = 0.000001f;
			const float4 tvec = r.mOrigin - centroid;
			const float4 pvec = cross(r.mDirection, edge2); 
			const float4 qvec = cross(tvec, edge1);
			float det, inv_det;


            rt::hit out;

            det = dot(edge1, pvec);
			//det = _mm_dp_ps(mEdge1.sseData, pvec.sseData, 0x7f)[0];

			if(det > -e && det < e)
				return out;
			inv_det = 1.0f / det;
			
            //float u = _mm_dp_ps(tvec.sseData, pvec.sseData, 0x7f)[0] * inv_det;
            float u = dot(tvec, pvec) * inv_det;
			if(u < 0.0f || u > 1.0f)
				return out;

			//float v = _mm_dp_ps(r->mSSEDirection, qvec.sseData, 0x7f)[0] * inv_det;
            float v = dot(r.mDirection, qvec) * inv_det;
			if(v < 0.0f || (u + v > 1.0f))
				return out;

			//float d = _mm_dp_ps(mEdge2.sseData, qvec.sseData, 0x7f)[0] * inv_det;
            float d = dot(edge2, qvec) * inv_det;
			if(d < e)
				return out;


			float4 hit = r.mDirection * float4(d) + r.mOrigin;
		    out.intersected = true;	
			out.position = float4(hit[0], hit[1], hit[2], 0.0f);
			out.normal = surfaceNormal;
			//out.tri = this;
			out.distance = d;
			return out;
		}

		float4 interpolatedNormal(const float4&)
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

		// glm::vec3 localToWorldVector(const float4& v)
		// {
		// 	__m128 vx = _mm_set1_ps(v.x);
		// 	__m128 vy = _mm_set1_ps(v.y);
		// 	__m128 vz = _mm_set1_ps(v.z);

		// 	vx = _mm_mul_ps(vx, sp1);
		// 	vy = _mm_mul_ps(vy, sn);
		// 	vz = _mm_mul_ps(vz, sp0);

		// 	__m128 o = _mm_add_ps(_mm_add_ps(vx, vy), vz);

		// 	return glm::vec3(o[0], o[1], o[2]);
		// }

		// __m128 localToWorldVector(const __m128& v)
		// {
		// 	__m128 vx = _mm_mul_ps(v, tx);
		// 	__m128 vy = _mm_mul_ps(v, ty);
		// 	__m128 vz = _mm_mul_ps(v, tz);

		// 	__m128 o;
		// 	o[0] = vx[0] + vx[1] + vx[2];
		// 	o[1] = vy[0] + vy[1] + vy[2];
		// 	o[2] = vz[0] + vz[1] + vz[2];
		// 	o[3] = 0.0f;

		// 	return o;
		// }
	};
}

