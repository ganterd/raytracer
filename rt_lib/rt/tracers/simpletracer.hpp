#include <rt/scene/scene.hpp>
#include <rt/scene/ray.hpp>
#include <rt/buffer/buffer.hpp>

namespace rt
{
	class SimpleRayTracer
	{
	public:
		void Trace(Scene* s, Buffer* b);
		void Trace(Scene* s, Buffer* b,	glm::ivec2 renderRegionMin,	glm::ivec2 renderRegionMax);

		glm::vec3 AccumulateLights(Scene* s, const glm::vec3& p, const glm::vec3& n);
		bool Shoot(Scene* s, const Ray& ray, RayHit& hit);
		bool Occluded(Scene* s, const Ray& ray, float d);
	};
}
