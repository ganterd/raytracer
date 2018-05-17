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

	public:
		void init(Scene* s);
		void Trace(Scene* s, Buffer* b);
		void Trace(Scene* s, Buffer* b,	glm::ivec2 renderRegionMin,	glm::ivec2 renderRegionMax);

	private:
		glm::vec3 AccumulateLights(Scene* s, const RayHit& p);
		bool Shoot(Scene* s, const Ray& ray, RayHit& hit);
		bool BVHTraversal(rt::BVHNode* n, const rt::Ray& ray, rt::RayHit& hit);
		bool Occluded(Scene* s, const Ray& ray, float d);
	};
}
