#include <rt/buffer/buffer.hpp>

#include <stdio.h>
#include <iostream>
#include <fstream>

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

void rt::Buffer::ToPPM(const char* path)
{
    FILE* pFile;

    std::fstream f = std::fstream(
        path,
        std::ios::out | std::ios::binary
    );

    f << std::string("P3") << std::endl;
    f << mSizex << " " << mSizey << " 255" << std::endl;

    for(int h = 0; h < mSizey; ++h)
    {
        for(int w = 0; w < mSizex; ++w)
        {
            uchar4 p = Pixel(w, h);
            f << std::to_string(p.r) << " ";
            f << std::to_string(p.g) << " ";
            f << std::to_string(p.b) << " ";
        }
        f << std::endl;
    }

    f.close();
}