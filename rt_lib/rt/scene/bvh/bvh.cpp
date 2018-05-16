#include <rt/scene/bvh/bvh.hpp>

rt::BVH::BVH()
{
    mNumBins = 512;
    mRoot = nullptr;
    mTargetScene = nullptr;
}

rt::BVH::~BVH()
{

}

void rt::BVH::construct(rt::Scene* scene)
{
    std::vector<Tri*> tris;
    tris.resize(scene->m_Tris.size());
    for(int i = 0; i < tris.size(); ++i)
    {
        tris[i] = &scene->m_Tris[i];
    }

    createBins();
    recursiveConstruct(tris, scene->mCentroidsAABB);
    freeBins();
    
    mTargetScene = scene;
}

void rt::BVH::createBins()
{
    mBins = new Bin*[3];
    for(int axis = 0; axis < 3; ++axis)
    {
        mBins[axis] = new Bin[mNumBins];
    }
}

void rt::BVH::freeBins()
{
    for(int axis = 0; axis < 3; ++axis)
    {
        delete[] mBins[axis];
    }

    delete[] mBins;
}

rt::BVHNode* rt::BVH::recursiveConstruct(const std::vector<Tri*>& tris, const AABB& centroidAABB)
{
    glm::vec3 aabbSize = centroidAABB.mMax - centroidAABB.mMin;

    // Initialise the bins
    for(int axis = 0; axis < 3; ++axis)
    {
        float sizePerBin = aabbSize[axis] / (float)mNumBins;
        for(int b = 0; b < mNumBins; ++b)
        {
            mBins[axis][b].mTris.clear();
            mBins[axis][b].mLeft = centroidAABB.mMax[axis] + (float)b * sizePerBin;
            mBins[axis][b].mRight = mBins[axis][b].mLeft + sizePerBin;
        }
    }

    // Bin all primitives to all 3 axis bins in one pass
    for(int t = 0; t < tris.size(); ++t)
    {
        Tri* tri = tris[t];
        for(int axis = 0; axis < 3; ++axis)
        {
            int axisBin = (float)mNumBins * (( 0.999999f * (tri->centroid[axis] - centroidAABB.mMin[axis])) / aabbSize[axis]);
            mBins[axis][axisBin].mTris.push_back(tri);
            mBins[axis][axisBin].mAABB.grow(tri->aabb);
        }
    }

    // Sweep from the right (on all axes) to generate the accumulated AABBs and costs
    rt::AABB totalAABBRight[3][mNumBins - 1];
    int totalPrimitivesRight[3][mNumBins - 1];

    rt::AABB prevAABB[3];
    int prevPrimitivesRight[3];

    prevAABB[0] = prevAABB[1] = prevAABB[2] = rt::AABB::infinity();
    prevPrimitivesRight[0] = prevPrimitivesRight[1] = prevPrimitivesRight[2] = 0;

    for(int b = mNumBins - 1; b >= 0; --b)
    {
        for(int ax = 0; ax < 3; ++ax)
        {
            prevAABB[ax].grow(mBins[ax][b].mAABB);
            totalAABBRight[ax][b] = prevAABB[ax];

            prevPrimitivesRight[ax] += mBins[ax][b].mTris.size();
            totalPrimitivesRight[ax][b] = prevPrimitivesRight[ax];
        }
    }

    // Sweep from left on all axes to find the best split
    int totalPrimitivesLeft[3];
    totalPrimitivesLeft[0] = totalPrimitivesLeft[1] = totalPrimitivesLeft[2] = 0;
    rt::AABB totalAABBLeft[3];
    totalAABBLeft[0] = totalAABBLeft[1] = totalAABBLeft[2] = rt::AABB::infinity();
    float bestCost = std::numeric_limits<float>::infinity();
    int bestCostSplit;
    int bestCostAxis;
    rt::AABB bestCostAABBLeft;
    rt::AABB bestCostAABBRight;
    for(int b = 0; b < mNumBins - 1; ++b)
    {
        for(int ax = 0; ax < 3; ++ax)
        {
            totalAABBLeft[ax].grow(mBins[ax][b].mAABB);
            totalPrimitivesLeft[ax] += mBins[ax][b].mTris.size();

            float costLeft = totalAABBLeft[ax].surfaceArea() * (float)totalPrimitivesLeft[ax];
            float costRight = totalAABBRight[ax][b + 1].surfaceArea() * (float)totalPrimitivesRight[ax][b + 1];
            float cost = costLeft + costRight;
            
            if(cost < bestCost)
            {
                bestCost = cost;
                bestCostAxis = ax;
                bestCostSplit = b;
                bestCostAABBLeft = totalAABBLeft[ax];
                bestCostAABBRight = totalAABBRight[ax][b + 1];
            }
        }
    }

    std::cout << "Best Cost Split: Axis[" << bestCostAxis << "], Split[" << bestCostSplit <<"], Cost "<< bestCost <<"]" << std::endl;
    std::cout << "|- Left AABB " << bestCostAABBLeft << std::endl;
    std::cout << "|- Right AABB " << bestCostAABBRight << std::endl;

    BVHNode* n = new BVHNode();
    return n;

    
}