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
    // Step 1: Determine binning axis
    int longestAxis;
    glm::vec3 aabbSize = scene->mCentroidsAABB.mMax - scene->mCentroidsAABB.mMin;

    // Initialise the bins
    Bin bins[3][512];
    for(int axis = 0; axis < 3; ++axis)
    {
        float sizePerBin = aabbSize[axis] / (float)mNumBins;
        std::cout << "Axis [" << axis << "] bin size: " << sizePerBin << std::endl;
        for(int b = 0; b < mNumBins; ++b)
        {
            bins[axis][b].mLeft = scene->mCentroidsAABB.mMax[axis] + (float)b * sizePerBin;
            bins[axis][b].mRight = bins[axis][b].mLeft + sizePerBin;
        }
    }

    // Bin all primitives to all 3 axis bins in one pass
    for(int t = 0; t < scene->m_Tris.size(); ++t)
    {
        Tri* tri = &scene->m_Tris[t];
        for(int axis = 0; axis < 3; ++axis)
        {
            int axisBin = (float)mNumBins * (( 0.999999f * (tri->centroid[axis] - scene->mCentroidsAABB.mMin[axis])) / aabbSize[axis]);
            bins[axis][axisBin].mTris.push_back(tri);
            bins[axis][axisBin].mAABB.grow(tri->aabb);
        }
    }
    
    for(int axis = 0; axis < 3; ++axis)
    {
        for(int b = 0; b < mNumBins; ++b)
        {
            std::cout << "Axis[" << axis << "] Bin[" << b << "] has " << bins[axis][b].mTris.size() << " tris ";
            std::cout << "AABB: [" << bins[axis][b].mAABB.mMin.x << "," << bins[axis][b].mAABB.mMin.y << "," << bins[axis][b].mAABB.mMin.z << "]->";
            std::cout << "[" << bins[axis][b].mAABB.mMax.x << "," << bins[axis][b].mAABB.mMax.y << "," << bins[axis][b].mAABB.mMax.z << "]";
            std::cout << std::endl;
        }
    }
    

    mTargetScene = scene;
}