#include <rt/tracers/simpletracer.hpp>

void rt::SimpleRayTracer::Trace(Scene* s, Buffer* b)
{
	// TMP: Fill with red.
	for(int y = 0; y < b->mSizey; ++y)
	{
		for(int x; x < b->mSizex; ++x)
		{
			glm::u8vec3 c(
				((float)x / (float)b->mSizex) * 255,
				((float)y / (float)b->mSizey) * 255,
				0
			);
			b->Pixel(x, y) = c;
		}
	}
}
