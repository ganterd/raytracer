#include <rt/buffer/buffer.hpp>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

void rt::buffer::toPPM(
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

    // [0,0] is lower left in our buffer, so here is where we
    // invert the image to output it.
    for(int h = mSizey - 1; h >= 0; --h)
    {
        for(int w = 0; w < mSizex; ++w)
        {
            float4 normalizedColor = pixel(w, h);
            float4 gammav = pow(normalizedColor, 1.0f / gamma);
            float4 scaled = clamp(gammav, 0.0f, 1.0f) * 255.0f;
            
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