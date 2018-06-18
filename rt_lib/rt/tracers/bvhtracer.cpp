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
	glm::ivec2 renderRegionMax,
	int threadIdx
){
	if(!s->mCamera)
		return;
	s->mCamera->SetAspectRatio((float)b->mSizex / (float)b->mSizey);

	renderRegionMax.x = glm::max(glm::min(renderRegionMax.x, b->mSizex), 0);
	renderRegionMax.y = glm::max(glm::min(renderRegionMax.y, b->mSizey), 0);

	for(int y = renderRegionMin.y; y < renderRegionMax.y; ++y)
	{
		for(int x = renderRegionMin.x; x < renderRegionMax.x; ++x)
		{
			rt::Ray* ray = (rt::Ray*)_mm_malloc(sizeof(rt::Ray), 32);
			*ray = s->mCamera->Ray(x, y, b->mSizex, b->mSizey);
			
			RayHit hit;
			glm::vec3 colour(0.0f);
			if(Shoot(ray, hit))
			{
				colour = AccumulateLights(hit, 0);
			}
		
			_mm_free(ray);

			b->SetPixel(x, y, colour, threadIdx);
		}
	}
}

bool rt::BVHRayTracer::Shoot(Ray* ray, RayHit& hit)
{
	if(bvh.cast(ray, hit))
	{
		hit.mInterpolatedNormal = hit.mTri->interpolatedNormal(hit.mHitPosition);
		if(glm::dot(hit.mSurfaceNormal, ray->direction()) > 0.0f)
		{
			hit.mSurfaceNormal *= -1.0f;
			hit.mInterpolatedNormal *= -1.0f;
		}
		return true;
	}
	return false;
}

bool rt::BVHRayTracer::Occluded(rt::Ray* ray, float distance)
{
	return bvh.occluded(ray, distance);
}

glm::vec3 rt::BVHRayTracer::AccumulateLights(const RayHit& p, int depth)
{
	glm::vec3 meshDiffuseColour(0.8f, 0.8f, 0.8f);
	glm::vec3 accumulatedColour = glm::vec3(0.0f);
	if(depth > mSettings.bounces)
		return accumulatedColour;

	__m128 hitPosition = _mm_set_ps(
		0.0f, 
		p.mHitPosition.z, 
		p.mHitPosition.y, 
		p.mHitPosition.x
	);

	__m128 hitNormal = _mm_set_ps(
		0.0f, 
		p.mInterpolatedNormal.z,
		p.mInterpolatedNormal.y,
		p.mInterpolatedNormal.x
	);

	__m128 hitBias = _mm_set1_ps(0.01f);

	__m128 biasPosition = _mm_fmadd_ps(hitNormal, hitBias, hitPosition);

	rt::Ray* ray = (rt::Ray*)_mm_malloc(sizeof(rt::Ray), 32);
	ray->setOrigin(biasPosition);
	
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
				if(!Occluded(&r, d))
				{
					float intensity = glm::max(dot * light->intensity(d), 0.0f);

					glm::vec3 meshDiffuseColour(0.8f, 0.8f, 0.8f);

					accumulatedColour += meshDiffuseColour * light->mColourDiffuse * intensity;
				}
			}
		}
		else if(lights[l]->mType == Light::Area)
		{
			AreaLight light = *(AreaLight*)lights[l];
			float sampleWeight = 1.0f / (float)light.mNumSamplePositions;
			if(glm::dot(light.mPosition - p.mHitPosition, light.mDirection) < 0.0f)
			{
				for(int i = 0; i < light.mNumSamplePositions; ++i)
				{	
					// 1: Make sure the surface can see the light (normal check)
					__m128 lightSample = light.sampleSSE(i);
					__m128 toLight = _mm_sub_ps(lightSample, hitPosition);

					// Get vector magnitude
					float d =_mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(toLight, toLight, 0x71)));

					// Normalize vector (v / |v|)
					toLight = _mm_div_ps(toLight, _mm_set1_ps(d));

					// Dot product of surface normal and light dir
					__m128 dp = _mm_mul_ps(hitNormal, toLight);
					dp = _mm_hadd_ps(dp, dp);
					dp = _mm_hadd_ps(dp, dp);
					float dot = dp[0];
					
					if(dot > 0.0f)
					{
						ray->setDirection(toLight);
						if(!Occluded(ray, d))
						{
							float intensity = dot * light.intensity(d);
							accumulatedColour += light.mColourDiffuse * intensity * sampleWeight;
						}
					}
				}
			}
		}
	}

	// Sample hemisphere for bounces
	glm::vec3 bouncedColour = accumulatedColour;
	float bounceWeight = 1.0f / (2.0f * M_PI) / (float)mSettings.bounceSamples;
	float randRcp = 1.0f / (float)RAND_MAX;
	for(int i = 0; i < mSettings.bounceSamples; ++i)
	{
		float rY = (float)std::rand() * randRcp;
		float rX = (float)std::rand() * randRcp;
		float theta = sqrtf(1.0f - rY * rY); 
		float phi = 2.0f * M_PI * rX; 
		float x = theta * cosf(phi); 
		float z = theta * sinf(phi); 

		__m128 sampleDirection;
		sampleDirection[0] = x;
		sampleDirection[1] = rX;
		sampleDirection[2] = z;
		sampleDirection[3] = 0.0f;
		float d = _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(sampleDirection, sampleDirection, 0x71)));
		sampleDirection = _mm_div_ps(sampleDirection, _mm_set1_ps(d));
		sampleDirection = p.mTri->localToWorldVector(sampleDirection);

		ray->setDirection(sampleDirection);
		RayHit hit;
		
		if(Shoot(ray, hit))
		{
			// Dot product of surface normal and light dir
			__m128 dp = _mm_mul_ps(hitNormal, sampleDirection);
			dp = _mm_hadd_ps(dp, dp);
			dp = _mm_hadd_ps(dp, dp);
			float dot = dp[0];

			bouncedColour += AccumulateLights(hit, depth + 1) * dot / glm::max(hit.mDistance, 1.0f) * meshDiffuseColour * bounceWeight;
		}
	}

	_mm_free(ray);

	return (accumulatedColour + bouncedColour) * meshDiffuseColour;
}
