#include <rt/buffer/buffer.hpp>


void rt::Buffer::SetPixel(
    int x,
    int y,
    const uchar4& c,
    int t)
{
    mThreadLocks[t].lock();
    Pixel(x, y) = c;
    mThreadLocks[t].unlock();
}

void rt::Buffer::SetPixel(
    int x,
    int y,
    const float4& normalisedColour,
    int t)
{
    float4 gamma = pow(normalisedColour, 1.0f / mGamma);
    float4 scaled = clamp(gamma, 0.0f, 1.0f) * 255.0f;
    
    uchar4 c;
    c.r = scaled.r;
    c.g = scaled.g;
    c.b = scaled.b;
    c.a = (char)255;

    mThreadLocks[t].lock();
    Pixel(x, y) = c;
    mThreadLocks[t].unlock();
}

void rt::Buffer::WriteLock()
{
    for(int i = 0; i < mNumThreadLocks; ++i)
        mThreadLocks[i].lock();
}

void rt::Buffer::WriteUnlock()
{
    for(int i = 0; i < mNumThreadLocks; ++i)
        mThreadLocks[i].unlock();
}
