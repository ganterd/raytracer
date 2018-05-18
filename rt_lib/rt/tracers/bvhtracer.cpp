#include <rt/tracers/bvhtracer.hpp>

void rt::BVHRayTracer::init(rt::Scene* s)
{
	if(bvh.mTargetScene != s)
	{
		bvh.construct(s);
	}
	mTargetScene = s;
}

void rt::BVHRayTracer::Trace(
	Scene* s,
	Buffer* b
){
	glm::ivec2 renderRegionMin(0, 0);
	glm::ivec2 renderRegionMax(b->mSizex, b->mSizey);
	Trace(s, b, renderRegionMin, renderRegionMax);
}

void rt::BVHRayTracer::Trace(
	Scene* s, 
	Buffer* b, 
	glm::ivec2 renderRegionMin, 
	glm::ivec2 renderRegionMax
){
	if(!s->mCamera)
		return;
	s->mCamera->SetAspectRatio((float)b->mSizex / (float)b->mSizey);

	renderRegionMax.x = glm::min(renderRegionMax.x, b->mSizex);
	renderRegionMax.y = glm::min(renderRegionMax.y, b->mSizey);

	for(int y = renderRegionMin.y; y < renderRegionMax.y; ++y)
	{
		for(int x = renderRegionMin.x; x < renderRegionMax.x; ++x)
		{
			Ray ray = s->mCamera->Ray(x, y, b->mSizex, b->mSizey);
			
			RayHit hit;
			glm::vec3 colour(0.0f);
			if(Shoot(ray, hit))
			{
				colour = AccumulateLights(hit);
			}

			// Gamma correction
			colour = glm::pow(colour, glm::vec3(1.0f / 2.2f));
			glm::u8vec4 c = glm::vec4(glm::clamp(colour, 0.0f, 1.0f), 1.0f) * 255.0f;
			b->Pixel(x, y) = c;
			
		}
	}
}

bool rt::BVHRayTracer::Shoot(const Ray& ray, RayHit& hit)
{
	if(bvh.cast(ray, hit))
	{
		hit.mInterpolatedNormal = hit.mTri->interpolatedNormal(hit.mHitPosition);
		if(glm::dot(hit.mSurfaceNormal, ray.mDirection) > 0.0f)
		{
			hit.mSurfaceNormal *= -1.0f;
			hit.mInterpolatedNormal *= -1.0f;
		}
		return true;
	}
	return false;
}

bool rt::BVHRayTracer::Occluded(const Ray& ray, float distance)
{
	return bvh.occluded(ray, distance);
}

glm::vec3 rt::BVHRayTracer::AccumulateLights(const RayHit& p, int depth)
{
	glm::vec3 accumulatedColour = glm::vec3(0.0f);
	if(depth > mSettings.bounces)
		return accumulatedColour;
	
	// Check all lights as if they were a point light for now
	int numLights = mTargetScene->mLights.size();
	Light** lights = &mTargetScene->mLights[0];
	for(int l = 0; l < numLights; ++l)
	{
		if(lights[l]->mType == Light::Point)
		{
			PointLight* light = (PointLight*)lights[l];

			// 1: Make sure the surface can see the light (normal check)
			glm::vec3 toLight = light->mPosition - p.mHitPosition;

			float d = glm::length(toLight);
			toLight = glm::normalize(toLight);
			float dot = glm::clamp(glm::dot(p.mInterpolatedNormal, toLight), 0.0f, 1.0f);
			if(dot > 0.0f)
			{
				Ray r(p.mHitPosition + p.mSurfaceNormal * 0.01f, toLight);
				if(!Occluded(r, d))
				{
					float intensity = glm::max(dot * light->intensity(d), 0.0f);

					glm::vec3 meshDiffuseColour(0.8f, 0.8f, 0.8f);

					accumulatedColour += meshDiffuseColour * light->mColourDiffuse * intensity;
				}
			}
		}
		else if(lights[l]->mType == Light::Area)
		{
			AreaLight* light = (AreaLight*)lights[l];
			float sampleWeight = 1.0f / (float)light->mNumSamplePositions;
			if(glm::dot(light->mPosition - p.mHitPosition, light->mDirection) < 0.0f)
			{
				for(int i = 0; i < light->mNumSamplePositions; ++i)
				{	
					// 1: Make sure the surface can see the light (normal check)
					glm::vec3 toLight = light->mSamplePositions[i] - p.mHitPosition;

					float d = glm::length(toLight);
					toLight = glm::normalize(toLight);
					float dot = glm::dot(p.mInterpolatedNormal, toLight);
					if(dot > 0.0f)
					{
						Ray r(p.mHitPosition + p.mSurfaceNormal * 0.01f, toLight);
						if(!Occluded(r, d))
						{
							float intensity = glm::max(dot * light->intensity(d), 0.0f);

							glm::vec3 meshDiffuseColour(0.8f, 0.8f, 0.8f);

							accumulatedColour += meshDiffuseColour * light->mColourDiffuse * intensity * sampleWeight;
						}
					}
				}
			}
		}
	}

	// Sample hemisphere for bounces
	glm::vec3 bouncedColour(0.0f);
	float bounceWeight = 1.0f / (float)mBounceSamples;
	for(int i = 0; i < mSettings.bounceSamples; ++i)
	{
		float rX = (((float)std::rand() / (float)RAND_MAX) * 180.0f) - 90.0f;
		float rY = (((float)std::rand() / (float)RAND_MAX) * 180.0f) - 90.0f;
		float rZ = (((float)std::rand() / (float)RAND_MAX) * 180.0f) - 90.0f;
		glm::vec3 sampleDir = p.mInterpolatedNormal;
		sampleDir = glm::rotateX(sampleDir, glm::radians(rX));
		sampleDir = glm::rotateY(sampleDir, glm::radians(rY));
		sampleDir = glm::rotateZ(sampleDir, glm::radians(rZ));

		Ray ray(p.mHitPosition + p.mSurfaceNormal * 0.01f, sampleDir);
		RayHit hit;
		
		if(Shoot(ray, hit))
		{
			float dot = glm::clamp(glm::dot(p.mInterpolatedNormal, sampleDir), 0.0f, 1.0f);
			bouncedColour += AccumulateLights(hit, depth + 1) * dot / glm::max(hit.mDistance * hit.mDistance, 1.0f);
		}
	}
	accumulatedColour += bouncedColour * bounceWeight;

	return accumulatedColour;
}
