#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <mutex>

namespace rt
{
	class Buffer
	{
	public:
		int mSizex;
		int mSizey;
		glm::u8vec4* mData;
		std::mutex* mThreadLocks;
		int mNumThreadLocks;

		Buffer(int x, int y, int threads = 1)
		{
			mData = new glm::u8vec4[x * y];
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

		void SetPixel(int x, int y, const glm::u8vec4& c, int t = 0)
		{
			mThreadLocks[t].lock();
			Pixel(x, y) = c;
			mThreadLocks[t].unlock();
		}

		void SetPixel(int x, int y, const glm::vec3& normalisedColour, int t = 0)
		{
			glm::u8vec4 c = glm::vec4(
				glm::clamp(
					glm::pow(normalisedColour, glm::vec3(1.0f / 2.2f)), 
					0.0f, 1.0f
				),
				1.0f
			) * 255.0f;
			mThreadLocks[t].lock();
			Pixel(x, y) = c;
			mThreadLocks[t].unlock();
		}

		void WriteLock()
		{
			for(int i = 0; i < mNumThreadLocks; ++i)
				mThreadLocks[i].lock();
		}

		void WriteUnlock()
		{
			for(int i = 0; i < mNumThreadLocks; ++i)
				mThreadLocks[i].unlock();
		}

	private:
		inline glm::u8vec4& Pixel(int x, int y)
		{
			int flipy = mSizey - 1 - y;
			return mData[x + mSizex * flipy];
		}
	};
}
