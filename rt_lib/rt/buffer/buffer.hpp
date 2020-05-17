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
		uchar4* mData;
		std::mutex* mThreadLocks;
		int mNumThreadLocks;
		float mGamma;

		Buffer(int x, int y, int threads = 1)
		{
			mGamma = 2.2f;
			mData = new uchar4[x * y];
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

		void SetPixel(int x, int y, const uchar4& c, int t = 0);

		void SetPixel(int x, int y, const float4& normalisedColour, int t = 0);

		void WriteLock();

		void WriteUnlock();

		inline uchar4& Pixel(int x, int y)
		{
			int flipy = mSizey - 1 - y;
			return mData[x + mSizex * flipy];
		}

		void ToPPM(const char* path);
	};
}
