#include "gtest/gtest.h"
#include <rt/buffer/buffer.hpp>

TEST(buffer, create)
{
    rt::buffer b(512, 512);
}

TEST(buffer, create_write_read_float4)
{
    rt::buffer b(256, 256);

    for(int h = 0; h < 256; ++h)
    {
        for(int w = 0; w < 256; ++w)
        {
            b.pixel(w, h) =  float4(
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
            float4 p = b.pixel(w, h);
            EXPECT_EQ(p.r, expected.r);
            EXPECT_EQ(p.g, expected.g);
            EXPECT_EQ(p.b, expected.b);
            EXPECT_EQ(p.a, expected.a);
        }
    }
}

TEST(buffer, write_to_ppm)
{
    rt::buffer b(256, 256);

    for(int h = 0; h < 256; ++h)
    {
        for(int w = 0; w < 256; ++w)
        {
            b.pixel(w, h) = float4(
                (float)w / (float)255,
                (float)h / (float)255,
                0.0f,
                1.0f
            );
        }
    }

    b.toPPM("file.ppm", 1.0f);
}