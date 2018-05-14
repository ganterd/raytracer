#include <rt/scene/scene.hpp>
#include <rt/buffer/buffer.hpp>

namespace rt
{
	class SimpleRayTracer
	{
	public:
		void Trace(Scene* s, Buffer* b);
		void Trace(Scene* s, Buffer* b,	glm::ivec2 renderRegionMin,	glm::ivec2 renderRegionMax);

		glm::vec3 AccumulateLights(Scene* s, const glm::vec3& p, const glm::vec3& n);
	};
}
