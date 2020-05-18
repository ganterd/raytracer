#pragma once

#include <iostream>
#include <mutex>
#include <rt/math/uchar4.hpp>
#include <rt/math/float4.hpp>

namespace rt
{
	class Buffer
	{
	public:
		int mSizex;
		int mSizey;
		float4* mData;
		float mGamma;

		Buffer(int x, int y)
		{
			mGamma = 2.2f;
			mData = new float4[x * y];
			mSizex = x;
			mSizey = y;
		}

		~Buffer()
		{
			delete[] mData;
		}

		inline float4& Pixel(int x, int y)
		{
			return mData[x + mSizex * y];
		}

		void ToPPM(const char* path, float gamma = 2.2f);
	};
}
