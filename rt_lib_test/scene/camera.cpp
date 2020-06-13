
#include "gtest/gtest.h"
#include <rt/scene/camera.hpp>
#include <rt/buffer/buffer.hpp>

TEST(camera, create)
{
    float4 position(0.0f, 0.0f, 1.0f, 1.0f);
    float4 lookAt(0.0f, 0.0f, 0.0f, 1.0f);
    float4 up(0.0f, 1.0f, 0.0f, 0.0f);

    rt::camera c(position, lookAt, up, 1.0f);
}

TEST(camera, precalculated_uv)
{
    float4 position(0.0f, 0.0f, 1.0f, 1.0f);
    float4 lookAt(0.0f, 0.0f, 0.0f, 1.0f);
    float4 up(0.0f, 1.0f, 0.0f, 0.0f);

    rt::camera c(position, lookAt, up, 1.0f);

    EXPECT_FLOAT_EQ(c.mU.x, 2.0f);
    EXPECT_FLOAT_EQ(c.mU.y, 0.0f);
    EXPECT_FLOAT_EQ(c.mU.z, 0.0f);
    EXPECT_FLOAT_EQ(c.mU.w, 0.0f);

    EXPECT_FLOAT_EQ(c.mV.x, 0.0f);
    EXPECT_FLOAT_EQ(c.mV.y, 2.0f);
    EXPECT_FLOAT_EQ(c.mV.z, 0.0f);
    EXPECT_FLOAT_EQ(c.mV.w, 0.0f);
}

TEST(camera, centre_ray)
{
    float4 position(0.0f, 0.0f, 1.0f, 1.0f);
    float4 lookAt(0.0f, 0.0f, 0.0f, 1.0f);
    float4 up(0.0f, 1.0f, 0.0f, 0.0f);

    rt::camera c(position, lookAt, up, 1.0f);

    rt::ray r = c.ray(0.5f, 0.5f);

    EXPECT_FLOAT_EQ(r.mOrigin.x, 0.0f);
    EXPECT_FLOAT_EQ(r.mOrigin.y, 0.0f);
    EXPECT_FLOAT_EQ(r.mOrigin.z, 1.0f);
    EXPECT_FLOAT_EQ(r.mOrigin.w, 1.0f);

    EXPECT_FLOAT_EQ(r.mDirection.x, 0.0f);
    EXPECT_FLOAT_EQ(r.mDirection.y, 0.0f);
    EXPECT_FLOAT_EQ(r.mDirection.z, -1.0f);
    EXPECT_FLOAT_EQ(r.mDirection.w, 0.0f);
}

TEST(camera, camera_vectors_negz_forward_file)
{
    float4 position(0.0f, 0.0f, 1.0f, 1.0f);
    float4 lookAt(0.0f, 0.0f, 0.0f, 1.0f);
    float4 up(0.0f, 1.0f, 0.0f, 0.0f);

    rt::camera c(position, lookAt, up, 1.0f);

    rt::ray r = c.ray(0.5f, 0.5f);

    /* Output the camera vectors to a buffer for subjective assesment */
    rt::buffer b(256, 256);

    for(int h = 0; h < b.mSizex; ++h)
    {
        float v = (float)h / (float)b.mSizey;
        for(int w = 0; w < b.mSizey; ++w)
        {
            float u = (float)w / (float)b.mSizex;
            r = c.ray(u, v);
            b.pixel(w, h) = r.mDirection;
        }
    }

    b.toPPM("camera_vectors_negz_forward.ppm", 1.0f);
}

TEST(camera, camera_vectors_posz_forward_file)
{
    float4 position(0.0f, 0.0f, -1.0f, 1.0f);
    float4 lookAt(0.0f, 0.0f, 0.0f, 1.0f);
    float4 up(0.0f, 1.0f, 0.0f, 0.0f);

    rt::camera c(position, lookAt, up, 1.0f);

    rt::ray r = c.ray(0.5f, 0.5f);

    /* Output the camera vectors to a buffer for subjective assesment */
    rt::buffer b(256, 256);

    for(int h = 0; h < b.mSizex; ++h)
    {
        float v = (float)h / (float)b.mSizey;
        for(int w = 0; w < b.mSizey; ++w)
        {
            float u = (float)w / (float)b.mSizex;
            r = c.ray(u, v);
            b.pixel(w, h) = r.mDirection;
        }
    }

    b.toPPM("camera_vectors_posz_forward.ppm", 1.0f);
}