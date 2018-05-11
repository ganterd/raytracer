#include <rt/tracers/simpletracer.hpp>

void rt::SimpleRayTracer::Trace(Scene* s, Buffer* b)
{
	// TMP: Fill with red.
	for(int y = 0; y < b->mSizey; ++y)
	{
		for(int x = 0; x < b->mSizex; ++x)
		{
			glm::u8vec4 c(
				((float)x / (float)b->mSizex) * 255,
				((float)y / (float)b->mSizey) * 255,
				0,
				255
			);
			b->Pixel(x, y) = c;
			
		}
	}
}
