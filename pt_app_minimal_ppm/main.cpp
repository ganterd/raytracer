#include <iostream>

#include <rt/scene/camera.hpp>
#include <rt/scene/tri.hpp>
#include <rt/buffer/buffer.hpp>

int main()
{
    rt::buffer buffer(512, 512);


    rt::camera camera(
        float4(0.0f, 0.0f, 5.0f, 1.0f),
        float4(0.0f, 0.0f, 0.0f, 1.0f),
        float4(0.0f, 1.0f, 0.0f, 0.0f),
        (float)buffer.mSizex / (float)buffer.mSizey
    );

    float4 v0(-1.0f, -1.0f, 1.0f, 1.0f);
    float4 v1( 1.0f, -1.0f, 1.0f, 1.0f);
    float4 v2( 0.0f,  1.0f, 1.0f, 1.0f);
    float4 n(0.0f, 0.0f, -1.0f, 0.0f);
    rt::tri t(v0, v2, v1, n, n, n);

    for(int h = 0; h < buffer.mSizex; ++h)
    {
        float v = (float)h / (float)buffer.mSizey;
        for(int w = 0; w < buffer.mSizey; ++w)
        {
            float u = (float)w / (float)buffer.mSizex;
            rt::ray r = camera.ray(u, v);

            rt::hit hit = t.intersect(r);

            float4 pixelColor(0.0f, 0.0f, 0.0f, 1.0f);
            if(hit.intersected)
            {
                pixelColor.r = 1.0f;
            }

            buffer.pixel(w, h) = pixelColor;
        }
    }
    
    buffer.toPPM("pt_app_minimal_ppm_output.ppm");
}