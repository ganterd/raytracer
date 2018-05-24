#include <rt/scene/bvh/bvh.hpp>

rt::BVH::BVH()
{
    mNumBins = 1024;
    mRoot = nullptr;
    mTargetScene = nullptr;
}

rt::BVH::~BVH()
{

}

void rt::BVH::construct(rt::Scene* scene)
{
    

    Tri** tris = new Tri*[scene->m_Tris.size()];
    for(unsigned int i = 0; i < scene->m_Tris.size(); ++i)
    {
        tris[i] = &scene->m_Tris[i];
    }

    mCurrentlyAllocatedNodes = 0;
    mAllocatedNodes = new rt::BVHNode[scene->m_Tris.size() * 2 - 1];
    //mAllocatedNodes = (BVHNode*)_mm_malloc(sizeof(BVHNode) * (scene->m_Tris.size() * 2 - 1), sizeof(__m256));

    deepestLevel = 0;
    numSplitNodes = 0;
    numLeafNodes = 0;

    std::cout << "Building BVH..." << std::endl;

    createBins();
    rt::Timer constructionTimer;
    constructionTimer.start();
    mRoot = recursiveConstruct(tris, scene->m_Tris.size(), scene->mCentroidsAABB, 0);
    constructionTimer.stop();
    freeBins();

    std::cout << "BVH Complete:" << std::endl;
    std::cout << "|- Time: " << constructionTimer.getTime() << "s" << std::endl;
    std::cout << "|- Deepest Level: " << deepestLevel << std::endl;
    std::cout << "|- Split Nodes: " << numSplitNodes << std::endl;
    std::cout << "|- Leaf Nodes: " << numLeafNodes << std::endl;
    std::cout << "|- Top AABB :" << mRoot->mAABB << std::endl;
    
    mTargetScene = scene;
}

rt::BVHNode* rt::BVH::newNode()
{
    return &mAllocatedNodes[mCurrentlyAllocatedNodes++];
}

void rt::BVH::createBins()
{
    mBins = new rt::BVH::Bin*[3];
    totalAABBRight = new rt::AABB*[3];
    totalCentroidAABBRight = new rt::AABB*[3];
    totalPrimitivesRight = new int*[3];

    for(int axis = 0; axis < 3; ++axis)
    {
        mBins[axis] = new rt::BVH::Bin[mNumBins];
        totalAABBRight[axis] = new rt::AABB[mNumBins];
        totalCentroidAABBRight[axis] = new rt::AABB[mNumBins];
        totalPrimitivesRight[axis] = new int[mNumBins];
    }
}

void rt::BVH::freeBins()
{
    for(int axis = 0; axis < 3; ++axis)
    {
        delete[] mBins[axis];
        delete[] totalAABBRight[axis];
        delete[] totalCentroidAABBRight[axis];
        delete[] totalPrimitivesRight[axis];
    }

    delete[] mBins;
    delete[] totalAABBRight;
    delete[] totalCentroidAABBRight;
    delete[] totalPrimitivesRight;
}

rt::BVHNode* rt::BVH::recursiveConstruct(Tri** tris, int numTris, const AABB& centroidAABB, int level)
{
    if(level > deepestLevel)
        deepestLevel = level;

    if(numTris <= BVHNode::mMaxTris)
    {
        BVHNode* n = &mAllocatedNodes[mCurrentlyAllocatedNodes++];
        n->mIsLeaf = true;
        for(int i = 0; i < numTris; ++i)
        {
            n->mTris[(size_t)n->mNumTris++] = tris[i];
            n->mAABB.grow(tris[i]->aabb);
        }
        numLeafNodes++;
        return n;
    }


    glm::vec3 aabbSize = centroidAABB.mSize;

    // Initialise the bins
    for(int axis = 0; axis < 3; ++axis)
    {
        float sizePerBin = aabbSize[axis] / (float)mNumBins;
        for(int b = 0; b < mNumBins; ++b)
        {
            mBins[axis][b].mTris.clear();
            mBins[axis][b].mTris.reserve(numTris / mNumBins);
            mBins[axis][b].mLeft = centroidAABB.mMax[axis] + (float)b * sizePerBin;
            mBins[axis][b].mRight = mBins[axis][b].mLeft + sizePerBin;
            mBins[axis][b].mAABB = rt::AABB::infinity();
            mBins[axis][b].mCentroidsAABB = rt::AABB::infinity();
        }
    }

    // Bin all primitives to all 3 axis bins in one pass
    for(int t = 0; t < numTris; ++t)
    {
        Tri* tri = tris[t];
        for(int axis = 0; axis < 3; ++axis)
        {
            int axisBin = 0;
            if(aabbSize[axis] > 0.0f)
                axisBin = (float)mNumBins * (( 0.9999f * (tri->centroid[axis] - centroidAABB.mMin[axis])) / aabbSize[axis]);
            mBins[axis][axisBin].mTris.push_back(tri);
            mBins[axis][axisBin].mAABB.grow(tri->aabb);
            mBins[axis][axisBin].mCentroidsAABB.grow(tri->centroid);
        }
    }

    // Find the best split
    BVHBestSplit best = findBestSplit();

    // Gather tris from left and right splits
    Tri** trisLeft = new Tri*[best.numPrimitives_L];
    Tri** trisRight = new Tri*[best.numPrimitives_R];

    int j = 0;
    for(int i = 0; i <= best.splitIndex; ++i)
    {
        for(unsigned int t = 0; t < mBins[best.axis][i].mTris.size(); ++t)
        {
            trisLeft[j++] = mBins[best.axis][i].mTris[t];
        }
    }

    j = 0;
    for(int i = best.splitIndex + 1; i < mNumBins; ++i)
    {
        for(unsigned int t = 0; t < mBins[best.axis][i].mTris.size(); ++t)
        {
            trisRight[j++] = mBins[best.axis][i].mTris[t];
        }
    }

    // Delete the incoming array (save space when recursing)
    delete[] tris;

    BVHNode* n = &mAllocatedNodes[mCurrentlyAllocatedNodes++];
    n->mLeft = recursiveConstruct(trisLeft, best.numPrimitives_L, best.centroidAABB_L, level + 1);
    n->mRight = recursiveConstruct(trisRight, best.numPrimitives_R, best.centroidAABB_R, level + 1);

    n->mAABB.grow(n->mLeft->mAABB);
    n->mAABB.grow(n->mRight->mAABB);
    
    numSplitNodes++;
    return n;
}

rt::BVHBestSplit rt::BVH::findBestSplit()
{
    BVHBestSplit best;

    // Sweep from the right (on all axes) to generate the accumulated AABBs and costs
    

    rt::AABB prevAABB[3];
    rt::AABB prevCentroidAABB[3];
    int prevPrimitivesRight[3];

    prevAABB[0] = prevAABB[1] = prevAABB[2] = rt::AABB::infinity();
    prevCentroidAABB[0] = prevCentroidAABB[1] = prevCentroidAABB[2] = rt::AABB::infinity();
    prevPrimitivesRight[0] = prevPrimitivesRight[1] = prevPrimitivesRight[2] = 0;

    for(int b = mNumBins - 1; b >= 0; --b)
    {
        for(int ax = 0; ax < 3; ++ax)
        {
            prevAABB[ax].grow(mBins[ax][b].mAABB);
            totalAABBRight[ax][b] = prevAABB[ax];

            prevCentroidAABB[ax].grow(mBins[ax][b].mCentroidsAABB);
            totalCentroidAABBRight[ax][b] = prevCentroidAABB[ax];

            prevPrimitivesRight[ax] += mBins[ax][b].mTris.size();
            totalPrimitivesRight[ax][b] = prevPrimitivesRight[ax];
        }
    }

    // Sweep from left on all axes to find the best split
    int totalPrimitivesLeft[3];
    totalPrimitivesLeft[0] = totalPrimitivesLeft[1] = totalPrimitivesLeft[2] = 0;
    rt::AABB totalAABBLeft[3];
    totalAABBLeft[0] = totalAABBLeft[1] = totalAABBLeft[2] = rt::AABB::infinity();
    rt::AABB totalCentroidAABBLeft[3];
    totalCentroidAABBLeft[0] = totalCentroidAABBLeft[1] = totalCentroidAABBLeft[2] = rt::AABB::infinity();

    for(int ax = 0; ax < 3; ++ax)
    {
        totalAABBLeft[ax].grow(mBins[ax][0].mAABB);
        totalCentroidAABBLeft[ax].grow(mBins[ax][0].mCentroidsAABB);
        totalPrimitivesLeft[ax] += mBins[ax][0].mTris.size();
    }

    for(int b = 1; b < mNumBins - 1; ++b)
    {
        int bRight = b + 1;
        for(int ax = 0; ax < 3; ++ax)
        {
            // Accumulate the AABBs and primitives count of the left bins
            totalAABBLeft[ax].grow(mBins[ax][b].mAABB);
            totalCentroidAABBLeft[ax].grow(mBins[ax][b].mCentroidsAABB);
            totalPrimitivesLeft[ax] += mBins[ax][b].mTris.size();

            // Determine the cost of this split
            float costLeft = totalAABBLeft[ax].surfaceArea() * (float)totalPrimitivesLeft[ax];
            float costRight = totalAABBRight[ax][bRight].surfaceArea() * (float)totalPrimitivesRight[ax][bRight];
            float cost = costLeft + costRight;
            
            // Replace teh current best cose with this one
            if(cost < best.cost && totalPrimitivesLeft[ax] > 0 && totalPrimitivesRight[ax][bRight] > 0)
            {
                best.cost = cost;
                best.axis = ax;
                best.splitIndex = b;
                best.AABBLeft = totalAABBLeft[ax];
                best.AABBRight = totalAABBRight[ax][bRight];
                best.centroidAABB_L = totalCentroidAABBLeft[ax];
                best.centroidAABB_R = totalCentroidAABBRight[ax][bRight];
                best.numPrimitives_L = totalPrimitivesLeft[ax];
                best.numPrimitives_R = totalPrimitivesRight[ax][bRight];
            }
        }
    }

    // std::cout << "Best Cost Split: Axis[" << best.axis << "], Split[" << best.splitIndex <<"], Cost "<< best.cost <<"]" << std::endl;
    // std::cout << "|- Left AABB " << best.AABBLeft << std::endl;
    // std::cout << "|- Right AABB " << best.AABBRight << std::endl;
    // std::cout << "|- Left Centroid AABB " << best.centroidAABB_L << std::endl;
    // std::cout << "|- Right Centroid AABB " << best.centroidAABB_R << std::endl;
    // std::cout << "|- Left/Right Primitives (" << best.numPrimitives_L << "/" << best.numPrimitives_R << ")" << std::endl;

    return best;
}

bool rt::BVH::cast(const rt::Ray& ray, rt::RayHit& hit)
{
    return cast(mRoot, ray, hit);
}

bool rt::BVH::cast(rt::BVHNode* n, const rt::Ray& ray, rt::RayHit& hit)
{
	if(n->mAABB.intersect(ray))
	{
		if(n->mIsLeaf)
		{
			bool hasHit = false;
			for(int t = 0; t < n->mNumTris; ++t)
			{
				RayHit currentHit;
				
				if(n->mTris[t]->rayIntersection(ray, currentHit))
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
			return cast(n->mLeft, ray, hit) | cast(n->mRight, ray, hit);
		}
	}
	return false;
}

bool rt::BVH::occluded(const rt::Ray& ray, const float distance)
{
    return occluded(mRoot, ray, distance);
}

bool rt::BVH::occluded(rt::BVHNode* n, const rt::Ray& ray, const float distance)
{
    if(n->mAABB.intersect(ray))
    {
        if(n->mIsLeaf)
        {
            for(int t = 0; t < n->mNumTris; ++t)
            {
                RayHit currentHit;
                if(n->mTris[t]->rayIntersection(ray, currentHit))
                {
                    if(currentHit.mDistance < distance)
                        return true;
                }
            }
        }
        else
        {
            if(occluded(n->mLeft, ray, distance))
                return true;
            else
                return occluded(n->mRight, ray, distance);
        }
    }
	return false;
}