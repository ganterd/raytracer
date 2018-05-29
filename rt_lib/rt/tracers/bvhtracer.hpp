#pragma once

#include <xmmintrin.h>
#include <immintrin.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <rt/scene/scene.hpp>
#include <rt/scene/ray.hpp>
#include <rt/buffer/buffer.hpp>
#include <rt/scene/bvh/bvh.hpp>
#include <rt/tracers/tracer.hpp>

namespace rt
{
	class BVHRayTracer : public rt::RayTracer
	{
	private:
		rt::Scene* mTargetScene;
		rt::BVH bvh;

		const int mMaxBounces = 2;
		const int mBounceSamples = 8;

	public:
		void init(Scene* s);
		void Trace(Scene* s, Buffer* b);
		void Trace(Scene* s, Buffer* b,	glm::ivec2 renderRegionMin,	glm::ivec2 renderRegionMax, int threadId = 0);

	private:
		glm::vec3 AccumulateLights(const RayHit& p, int depth = 0, Buffer* b = nullptr, const glm::ivec2& currentPixel = glm::ivec2(0));
		bool Shoot(const Ray& ray, RayHit& hit);
		bool Occluded(const Ray& ray, float d);
	};
}
