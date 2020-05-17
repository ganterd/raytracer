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
		std::mutex* mThreadLocks;
		int mNumThreadLocks;
		float mGamma;

		Buffer(int x, int y, int threads = 1)
		{
			mGamma = 2.2f;
			mData = new float4[x * y];
			mSizex = x;
			mSizey = y;
			mNumThreadLocks = threads;
			mThreadLocks = new std::mutex[threads];
		}

		~Buffer()
		{
			delete[] mData;
			delete[] mThreadLocks;
		}

		inline float4& Pixel(int x, int y)
		{
			int flipy = mSizey - 1 - y;
			return mData[x + mSizex * flipy];
		}

		void ToPPM(const char* path, float gamma = 2.2f);
	};
}
