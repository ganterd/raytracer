#include <rt/buffer/buffer.hpp>

#include <stdio.h>
#include <iostream>
#include <fstream>

void rt::Buffer::ToPPM(
    const char* path,
    float gamma
){
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
            float4 normalizedColor = Pixel(w, h);
            float4 gamma = pow(normalizedColor, 1.0f / mGamma);
            float4 scaled = clamp(gamma, 0.0f, 1.0f) * 255.0f;
            
            uchar4 c;
            c.r = scaled.r;
            c.g = scaled.g;
            c.b = scaled.b;

            f << std::to_string(c.r) << " ";
            f << std::to_string(c.g) << " ";
            f << std::to_string(c.b) << " ";
        }
        f << std::endl;
    }

    f.close();
}