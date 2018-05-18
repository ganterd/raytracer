#pragma once

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

		const int mMaxBounces = 0;
		const int mBounceSamples = 8;

	public:
		void init(Scene* s);
		void Trace(Scene* s, Buffer* b);
		void Trace(Scene* s, Buffer* b,	glm::ivec2 renderRegionMin,	glm::ivec2 renderRegionMax);

	private:
		glm::vec3 AccumulateLights(Scene* s, const RayHit& p, int depth = 0);
		bool Shoot(Scene* s, const Ray& ray, RayHit& hit);
		bool BVHTraversal(rt::BVHNode* n, const rt::Ray& ray, rt::RayHit& hit);
		bool Occluded(Scene* s, const Ray& ray, float d);
	};
}
