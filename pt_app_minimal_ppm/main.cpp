#include <iostream>

#include <rt/scene/camera.hpp>
#include <rt/scene/tri.hpp>
#include <rt/buffer/buffer.hpp>
#include <rt/scene/scene.hpp>
#include <rt/scene/bvh/bvh.hpp>
#include <rt/utils/timer.hpp>

int main(int argc, char* argv[])
{
    rt::buffer buffer(256, 256);


    rt::camera camera(
        float4(0.0f, 0.0f, 5.0f, 1.0f),
        float4(0.0f, 0.0f, 0.0f, 1.0f),
        float4(0.0f, 1.0f, 0.0f, 0.0f),
        (float)buffer.mSizex / (float)buffer.mSizey
    );

    rt::timer timer;
    timer.start();
    rt::scene scene;
    scene.fromFile(argv[1]);
    timer.stop();
    std::cout << "Loaded '" << argv[1] << "' (" << timer.getTime() << "s)" << std::endl;

    rt::bvh bvh;
    bvh.construct(&scene);

    timer.start();
    std::cout << "Tracing: ";
    for(int h = 0; h < buffer.mSizey; ++h)
    {
        std::cout << "\rTracing Row [" << h << "]" << std::flush;

        float v = (float)h / (float)buffer.mSizey;
        for(int w = 0; w < buffer.mSizex; ++w)
        {
            float u = (float)w / (float)buffer.mSizex;
            rt::ray r = scene.mCamera->ray(u, v);

            float4 pixelColor(0.0f, 0.0f, 0.0f, 1.0f);

            rt::hit hit = bvh.cast(r);
            if(hit.intersected)
            {
                float scaled = hit.distance * 0.01f;
                pixelColor = point(float4(scaled));
            }

            buffer.pixel(w, h) = pixelColor;
        }
    }
    timer.stop();
    std::cout << "\rFinished (" << timer.getTime() << "s)" << std::endl;

    timer.start();
    buffer.toPPM("pt_app_minimal_ppm_output.ppm");
    timer.stop();
    std::cout << "Output to file (" << timer.getTime() << "s)" << std::endl;
}