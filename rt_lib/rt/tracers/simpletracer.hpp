#include <rt/scene/scene.hpp>
#include <rt/scene/ray.hpp>
#include <rt/buffer/buffer.hpp>
#include <rt/tracers/tracer.hpp>

namespace rt
{
	class SimpleRayTracer : public rt::RayTracer
	{
	public:
		void Trace(Scene* s, Buffer* b);
		void Trace(Scene* s, Buffer* b,	glm::ivec2 renderRegionMin,	glm::ivec2 renderRegionMax, int threadId = 0);

		glm::vec3 AccumulateLights(Scene* s, const RayHit& p);
		bool Shoot(Scene* s, const Ray& ray, RayHit& hit);
		bool Occluded(Scene* s, const Ray& ray, float d);
	};
}
