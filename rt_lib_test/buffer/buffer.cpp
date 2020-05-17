#include "gtest/gtest.h"
#include <rt/buffer/buffer.hpp>

TEST(Buffer, create)
{
    rt::Buffer b(512, 512);
}

TEST(Buffer, create_write_read_uchar4)
{
    rt::Buffer b(512, 512);

    for(int h = 0; h < 512; ++h)
    {
        for(int w = 0; w < 512; ++w)
        {
            b.SetPixel(w, h, uchar4(
                (char)w,
                (char)h,
                (char)0,
                (char)255
            ));
        }
    }

    for(int h = 0; h < 512; ++h)
    {
        for(int w = 0; w < 512; ++w)
        {
            uchar4 p = b.Pixel(w, h);
            EXPECT_EQ(p.r, (char)w);
            EXPECT_EQ(p.g, (char)h);
            EXPECT_EQ(p.b, (char)0);
            EXPECT_EQ(p.a, (char)255);
        }
    }
}

TEST(Buffer, create_write_read_float4)
{
    rt::Buffer b(256, 256);
    b.mGamma = 1.0f;

    for(int h = 0; h < 256; ++h)
    {
        for(int w = 0; w < 256; ++w)
        {
            b.SetPixel(w, h, float4(
                (float)w / (float)255,
                (float)h / (float)255,
                0.0f,
                10.0f
            ));
        }
    }

    for(int h = 0; h < 256; ++h)
    {
        for(int w = 0; w < 256; ++w)
        {
            uchar4 p = b.Pixel(w, h);
            EXPECT_EQ(p.r, (char)w);
            EXPECT_EQ(p.g, (char)h);
            EXPECT_EQ(p.b, (char)0);
            EXPECT_EQ(p.a, (char)255);
        }
    }
}