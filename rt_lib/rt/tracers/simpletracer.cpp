#include <rt/tracers/simpletracer.hpp>

void rt::SimpleRayTracer::Trace(Scene* s, Buffer* b)
{
	// TMP: Fill with red.
	for(int y = 0; y < b->mSizey; ++y)
	{
		for(int x = 0; x < b->mSizex; ++x)
		{
			glm::vec3 rayDirection;
			if(s->mCamera)
			{
				rayDirection = s->mCamera->Ray(x, y, b->mSizex, b->mSizey);
				std::cout << rayDirection.x <<"," << rayDirection.y << "," << rayDirection.z << std::endl;
			}
			
			glm::u8vec4 c(
				rayDirection.x * 255.0f,
				rayDirection.y * 255.0f,
				rayDirection.z * 255.0f,
				255
			);
			b->Pixel(x, y) = c;
			
		}
	}
}
