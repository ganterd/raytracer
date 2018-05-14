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
	glm::ivec2 renderRegionMin, 
	glm::ivec2 renderRegionMax
){
	s->mCamera->SetAspectRatio((float)b->mSizex / (float)b->mSizey);

	renderRegionMax.x = glm::min(renderRegionMax.x, b->mSizex);
	renderRegionMax.y = glm::min(renderRegionMax.y, b->mSizey);
	const float maxDepth = 10000.0f;

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
			float currentDepth = maxDepth;
			glm::vec3 currentPosition(0.0f);
			glm::vec3 currentNormal(0.0f);
			for(int i = 0; i < s->m_Tris.size(); ++i)
			{
				float hitDistance;
				glm::vec3 hitPosition;
				glm::vec3 hitNormal;
				if(s->m_Tris[i].rayIntersection(rayOrigin, rayDirection, hitDistance, hitPosition, hitNormal))
				{
					if(hitDistance < currentDepth)
					{
						currentDepth = hitDistance;
						currentPosition = hitPosition;
						if(glm::dot(hitNormal, rayDirection) > 0.0f)
							currentNormal = -hitNormal;
						else
							currentNormal = hitNormal;
					}
				}
			}

			glm::vec3 colour(0.0f);
			if(currentDepth != maxDepth)
			{
				colour = AccumulateLights(s, currentPosition, currentNormal);
			}

			//glm::u8vec4 c = glm::vec4(rayDirection, 1.0f) * 255.0f;
			//glm::u8vec4 c = glm::vec4(glm::clamp(currentPosition * 0.1f, 0.0f, 1.0f), 1.0f) * 255.0f;
			//glm::u8vec4 c = glm::vec4(glm::clamp(glm::vec3(currentDepth * 0.01f), 0.0f, 1.0f), 1.0f) * 255.0f;
			glm::u8vec4 c = glm::vec4(glm::clamp(colour, 0.0f, 1.0f), 1.0f) * 255.0f;
			b->Pixel(x, y) = c;
		}
	}
}

glm::vec3 rt::SimpleRayTracer::AccumulateLights(Scene* s, const glm::vec3& p, const glm::vec3& n)
{
	glm::vec3 accumulatedColour = glm::vec3(0.0f);
	
	// Check all lights as if they were a point light for now
	for(int l = 0; l < s->mLights.size(); ++l)
	{
		PointLight* light = (PointLight*)s->mLights[l];

		// 1: Make sure the surface can see the light (normal check)
		glm::vec3 toLight = light->mPosition - p;

		float d = glm::length(toLight);
		toLight = glm::normalize(toLight);
		float dot = glm::clamp(glm::dot(n, toLight), 0.0f, 1.0f);
		if(dot > 0.0f)
		{
			glm::vec3 biasPosition = p + n * 0.01f;
			bool canSeeLight = true;
			for(int i = 0; i < s->m_Tris.size() && canSeeLight; ++i)
			{
				float hitDistance;
				glm::vec3 hitPosition;
				glm::vec3 hitNormal;
				if(s->m_Tris[i].rayIntersection(biasPosition, toLight, hitDistance, hitPosition, hitNormal))
				{
					if(hitDistance < d)
						canSeeLight = false;
				}
			}

			if(canSeeLight)
			{
				float intensity = glm::max(dot * light->intensity(d), 0.0f);

				glm::vec3 meshDiffuseColour(0.8f, 0.8f, 0.8f);

				accumulatedColour += meshDiffuseColour * light->mColourDiffuse * intensity;
			}
		}
	}



	return glm::pow(accumulatedColour, glm::vec3(1.0f / 2.2f));
}
