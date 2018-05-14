#include <rt/tracers/simpletracer.hpp>

void rt::SimpleRayTracer::Trace(
	Scene* s,
	Buffer* b
){
	glm::ivec2 renderRegionMin(0, 0);
	glm::ivec2 renderRegionMax(b->mSizex, b->mSizey);
	Trace(s, b, renderRegionMin, renderRegionMax);
}

void rt::SimpleRayTracer::Trace(
	Scene* s, 
	Buffer* b, 
	const glm::ivec2& renderRegionMin, 
	const glm::ivec2& renderRegionMax
){
	s->mCamera->SetAspectRatio((float)b->mSizex / (float)b->mSizey);

	for(int y = renderRegionMin.y; y < renderRegionMax.y; ++y)
	{
		for(int x = renderRegionMin.x; x < renderRegionMax.x; ++x)
		{
			glm::vec3 rayDirection;
			glm::vec3 rayOrigin;
			if(s->mCamera)
			{
				rayDirection = s->mCamera->Ray(x, y, b->mSizex, b->mSizey);
				rayOrigin = s->mCamera->mPosition;
			}
			
			// Really really simple brute force. I'll make this better. I promise.
			float currentDepth = 10000.0f;
			glm::vec3 currentPosition(0);
			for(int i = 0; i < s->m_Tris.size(); ++i)
			{
				float triHitDistance;
				glm::vec3 triHitPosition;

				if(s->m_Tris[i].rayIntersection(rayOrigin, rayDirection, triHitDistance, triHitPosition))
				{
					if(triHitDistance < currentDepth)
					{
						currentDepth = triHitDistance;
						currentPosition = triHitPosition;
					}
				}
			}

			//glm::u8vec4 c = glm::vec4(rayDirection, 1.0f) * 255.0f;
			//glm::u8vec4 c = glm::vec4(glm::clamp(currentPosition * 0.1f, 0.0f, 1.0f), 1.0f) * 255.0f;
			glm::u8vec4 c = glm::vec4(glm::clamp(glm::vec3(currentDepth * 0.05f), 0.0f, 1.0f), 1.0f) * 255.0f;
			b->Pixel(x, y) = c;
		}
	}
}
