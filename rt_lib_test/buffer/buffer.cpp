#include "gtest/gtest.h"
#include <rt/buffer/buffer.hpp>

TEST(Buffer, create)
{
    rt::Buffer b(512, 512);
}

TEST(Buffer, create_write_read_float4)
{
    rt::Buffer b(256, 256);
    b.mGamma = 1.0f;

    for(int h = 0; h < 256; ++h)
    {
        for(int w = 0; w < 256; ++w)
        {
            b.Pixel(w, h) =  float4(
                (float)w / (float)255,
                (float)h / (float)255,
                0.0f,
                1.0f
            );
        }
    }

    for(int h = 0; h < 256; ++h)
    {
        for(int w = 0; w < 256; ++w)
        {
            float4 expected(
                (float)w / (float)255,
                (float)h / (float)255,
                0.0f,
                1.0f
            );
            float4 p = b.Pixel(w, h);
            EXPECT_EQ(p.r, expected.r);
            EXPECT_EQ(p.g, expected.g);
            EXPECT_EQ(p.b, expected.b);
            EXPECT_EQ(p.a, expected.a);
        }
    }
}

TEST(Buffer, write_to_ppm)
{
    rt::Buffer b(256, 256);
    b.mGamma = 1.0f;

    for(int h = 0; h < 256; ++h)
    {
        for(int w = 0; w < 256; ++w)
        {
            b.Pixel(w, h) = float4(
                (float)w / (float)255,
                (float)h / (float)255,
                0.0f,
                1.0f
            );
        }
    }

    b.ToPPM("file.ppm");
}