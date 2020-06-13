#include <rt/scene/bvh/bvh.hpp>

rt::bvh::bvh()
{
    mNumBins = 1024;
    mRoot = nullptr;
    mTargetScene = nullptr;
}

rt::bvh::~bvh()
{

}

void rt::bvh::construct(rt::scene* scene)
{
    rt::tri** tris = new rt::tri*[scene->m_TotalTris];
    for(unsigned int i = 0; i < scene->m_TotalTris; ++i)
    {
        tris[i] = &scene->m_Tris[i];
    }

    mCurrentlyAllocatedNodes = 0;
    //mAllocatedNodes = new rt::BVHNode[scene->m_Tris.size() * 2 - 1];
    size_t allocationSize = (scene->m_TotalTris * 2 - 1) * sizeof(rt::bvh_node) + sizeof(rt::bvh_node);
    mAllocatedNodes = (rt::bvh_node*)_mm_malloc(allocationSize, 32);

    deepestLevel = 0;
    numSplitNodes = 0;
    numLeafNodes = 0;

    std::cout << "Building BVH..." << std::endl;

    createBins();
    rt::timer constructionTimer;
    constructionTimer.start();
    mRoot = recursiveConstruct(tris, scene->m_TotalTris, scene->mCentroidsAABB, 0);
    constructionTimer.stop();
    freeBins();

    std::cout << "BVH Complete " << constructionTimer.getTime() << "s):" << std::endl;
    std::cout << "|- Deepest Level: " << deepestLevel << std::endl;
    std::cout << "|- Split Nodes: " << numSplitNodes << std::endl;
    std::cout << "|- Leaf Nodes: " << numLeafNodes << std::endl;
    std::cout << "|- Top AABB :" << mRoot->mAABB << std::endl;
    
    mTargetScene = scene;
}

rt::bvh_node* rt::bvh::newNode()
{
    return &mAllocatedNodes[mCurrentlyAllocatedNodes++];
}

void rt::bvh::createBins()
{
    mBins = new rt::bvh::bin*[3];
    totalAABBRight = new rt::aabb*[3];
    totalCentroidAABBRight = new rt::aabb*[3];
    totalPrimitivesRight = new int*[3];

    for(int axis = 0; axis < 3; ++axis)
    {
        mBins[axis] = new rt::bvh::bin[mNumBins];
        totalAABBRight[axis] = new rt::aabb[mNumBins];
        totalCentroidAABBRight[axis] = new rt::aabb[mNumBins];
        totalPrimitivesRight[axis] = new int[mNumBins];
    }
}

void rt::bvh::freeBins()
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

rt::bvh_node* rt::bvh::recursiveConstruct(
    rt::tri** tris,
    int numTris,
    const rt::aabb& centroidAABB,
    int level)
{
    if(level > deepestLevel)
    {
        deepestLevel = level;
    }

    if(numTris <= bvh_node::mMaxTris)
    {
        rt::bvh_node* n = &mAllocatedNodes[mCurrentlyAllocatedNodes++];
        n->mIsLeaf = true;
        n->mNumTris = 0;
        for(int i = 0; i < numTris; ++i)
        {
            n->mTris[(size_t)n->mNumTris++] = tris[i];

            n->mAABB.grow(tris[i]->v0);
            n->mAABB.grow(tris[i]->v1);
            n->mAABB.grow(tris[i]->v2);
        }
        numLeafNodes++;
        delete[] tris;
        return n;
    }


    float4 aabbSize = centroidAABB.bsize;

    // Initialise the bins
    for(int axis = 0; axis < 3; ++axis)
    {
        float sizePerBin = aabbSize[axis] / (float)mNumBins;
        for(int b = 0; b < mNumBins; ++b)
        {
            mBins[axis][b].mTris.clear();
            mBins[axis][b].mTris.reserve(numTris / mNumBins);
            mBins[axis][b].mLeft = centroidAABB.bmax[axis] + (float)b * sizePerBin;
            mBins[axis][b].mRight = mBins[axis][b].mLeft + sizePerBin;
            mBins[axis][b].mAABB = rt::aabb::infinity();
            mBins[axis][b].mCentroidsAABB = rt::aabb::infinity();
        }
    }

    // Bin all primitives to all 3 axis bins in one pass
    for(int t = 0; t < numTris; ++t)
    {
        rt::tri *tri = tris[t];
        for(int axis = 0; axis < 3; ++axis)
        {
            int axisBin = 0;
            if(aabbSize[axis] > 0.0f)
                axisBin = (float)mNumBins * (( 0.9999f * (tri->centroid[axis] - centroidAABB.bmin[axis])) / aabbSize[axis]);
            mBins[axis][axisBin].mTris.push_back(tri);
            mBins[axis][axisBin].mAABB.grow(tri->v0);
            mBins[axis][axisBin].mAABB.grow(tri->v1);
            mBins[axis][axisBin].mAABB.grow(tri->v2);
            mBins[axis][axisBin].mCentroidsAABB.grow(tri->centroid);
        }
    }

    // Find the best split
    bvh_best_split best = findBestSplit();

    // Gather tris from left and right splits
    rt::tri **trisLeft = new rt::tri*[best.numPrimitives_L];
    rt::tri **trisRight = new rt::tri*[best.numPrimitives_R];

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

    bvh_node* n = &mAllocatedNodes[mCurrentlyAllocatedNodes++];
    n->mIsLeaf = false;
    n->AssignLeft(recursiveConstruct(trisLeft, best.numPrimitives_L, best.centroidAABB_L, level + 1));
    n->AssignRight(recursiveConstruct(trisRight, best.numPrimitives_R, best.centroidAABB_R, level + 1));
    numSplitNodes++;
    return n;
}

rt::bvh_best_split rt::bvh::findBestSplit()
{
    bvh_best_split best;

    // Sweep from the right (on all axes) to generate the accumulated AABBs and costs
    

    rt::aabb prevAABB[3];
    rt::aabb prevCentroidAABB[3];
    int prevPrimitivesRight[3];

    prevAABB[0] = prevAABB[1] = prevAABB[2] = rt::aabb::infinity();
    prevCentroidAABB[0] = prevCentroidAABB[1] = prevCentroidAABB[2] = rt::aabb::infinity();
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
    rt::aabb totalAABBLeft[3];
    totalAABBLeft[0] = totalAABBLeft[1] = totalAABBLeft[2] = rt::aabb::infinity();
    rt::aabb totalCentroidAABBLeft[3];
    totalCentroidAABBLeft[0] = totalCentroidAABBLeft[1] = totalCentroidAABBLeft[2] = rt::aabb::infinity();

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

    return best;
}

rt::hit rt::bvh::cast(const rt::ray &ray)
{
    rt::hit hit;
    cast(mRoot, ray, hit);
    return hit;
}

void rt::bvh::cast(rt::bvh_node *n, const rt::ray &ray, rt::hit &hit)
{
    if(n == nullptr)
    {
        return;
    }

    rt::hit aabbHit = n->mAABB.intersect(ray);
    if(aabbHit.intersected && aabbHit.distance < hit.distance)
    {
        if(n->mIsLeaf)
        {
            bool hasHit = false;
            for(int t = 0; t < n->mNumTris; ++t)
            {
                rt::hit currentHit = n->mTris[t]->intersect(ray);
                if(currentHit.intersected)
                {
                    hasHit = true;
                    if(currentHit.distance < hit.distance)
                    {
                        hit = currentHit;
                    }
                }
            }
        }
        else
        {
            cast(n->mLeft, ray, hit);
            cast(n->mRight, ray, hit);
        }
    }
}

// bool rt::BVH::occluded(rt::Ray* ray, const float distance)
// {
//     //return occluded(mRoot, ray, distance);
//     return occludedDual(mRoot, ray, distance);
// }

// bool rt::BVH::occludedDual(rt::BVHNode* n, rt::Ray* ray, const float distance)
// {
//     if(n->mIsLeaf)
//     {
//         for(int t = 0; t < n->mNumTris; ++t)
//         {
//             RayHit currentHit;
//             if(n->mTris[t]->rayIntersection(ray, currentHit))
//             {
//                 if(currentHit.mDistance < distance)
//                     return true;
//             }
//         }
//     }
//     else
//     {

//         bool left, right;
//         n->intersectBothChildren(ray, left, right);
//         if(left)
//             if(occludedDual(n->mLeft, ray, distance))
//                 return true;
//         if(right)
//             if(occludedDual(n->mRight, ray, distance))
//                 return true;
//     }
// 	return false;
// }
