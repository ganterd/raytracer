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
	const float maxDepth = 10000.0f;

	for(int y = renderRegionMin.y; y < renderRegionMax.y; ++y)
	{
		for(int x = renderRegionMin.x; x < renderRegionMax.x; ++x)
		{
			Ray ray = s->mCamera->Ray(x, y, b->mSizex, b->mSizey);
			
			// Really really simple brute force. I'll make this better. I promise.
			RayHit hit;
			glm::vec3 colour(0.0f);
			if(Shoot(s, ray, hit))
			{
				colour = AccumulateLights(s, hit);
			}

			colour = glm::pow(colour, glm::vec3(1.0f / 2.2f));
			glm::u8vec4 c = glm::vec4(glm::clamp(colour, 0.0f, 1.0f), 1.0f) * 255.0f;
			b->Pixel(x, y) = c;
			
		}
	}
}

bool rt::BVHRayTracer::Shoot(Scene* s, const Ray& ray, RayHit& hit)
{
	BVHTraversal(bvh.mRoot, ray, hit);

	if(hit.mTri)
	{
		hit.mInterpolatedNormal = hit.mTri->interpolatedNormal(hit.mHitPosition);
		if(glm::dot(hit.mSurfaceNormal, ray.mDirection) > 0.0f)
		{
			hit.mSurfaceNormal *= -1.0f;
			hit.mInterpolatedNormal *= -1.0f;
		}
	}

	return hit.mDistance != std::numeric_limits<float>::infinity();
}

bool rt::BVHRayTracer::BVHTraversal(rt::BVHNode* n, const rt::Ray& ray, rt::RayHit& hit)
{
	if(n->mAABB.intersect(ray))
	{
		if(n->mIsLeaf)
		{
			BVHLeafNode* leaf = (BVHLeafNode*)n;
			bool hasHit = false;
			for(int t = 0; t < leaf->mNumTris; ++t)
			{
				RayHit currentHit;
				
				if(leaf->mTris[t]->rayIntersection(ray, currentHit))
				{
					hasHit = true;
					if(currentHit.mDistance < hit.mDistance)
					{
						hit = currentHit;
					}
				}
			}
			return hasHit;
		}
		else
		{
			return BVHTraversal(n->mLeft, ray, hit) | BVHTraversal(n->mRight, ray, hit);
		}
	}
	return false;
}

bool rt::BVHRayTracer::Occluded(Scene* s, const Ray& ray, float distance)
{
	RayHit hit;
	BVHTraversal(bvh.mRoot, ray, hit);
	return hit.mDistance < distance;
}

glm::vec3 rt::BVHRayTracer::AccumulateLights(Scene* s, const RayHit& p)
{
	glm::vec3 accumulatedColour = glm::vec3(0.0f);
	
	// Check all lights as if they were a point light for now
	int numLights = s->mLights.size();
	Light** lights = &s->mLights[0];

	int numTris = s->m_Tris.size();
	Tri* tris = &s->m_Tris[0];

	for(int l = 0; l < numLights; ++l)
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
			if(!Occluded(s, r, d))
			{
				float intensity = glm::max(dot * light->intensity(d), 0.0f);

				glm::vec3 meshDiffuseColour(0.8f, 0.8f, 0.8f);

				accumulatedColour += meshDiffuseColour * light->mColourDiffuse * intensity;
			}
		}
	}



	return accumulatedColour;
}
