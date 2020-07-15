#pragma once

#include <iostream>
#include <mutex>
#include <rt/math/uchar4.hpp>
#include <rt/math/float4.hpp>
#include <rt/utils/export.hpp>

namespace rt
{
	class buffer
	{
	public:
		int mSizex;
		int mSizey;
		float4* mData;

		rt_lib buffer(int x, int y)
		{
			mData = new float4[x * y];
			mSizex = x;
			mSizey = y;
		}

		rt_lib ~buffer()
		{
			delete[] mData;
		}

		inline float4& pixel(int x, int y)
		{
			return mData[x + mSizex * y];
		}

		rt_lib void toPPM(const char* path, float gamma = 2.2f);
	};
}
