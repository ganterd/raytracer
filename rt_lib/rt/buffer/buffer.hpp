#pragma once

#include <glm/glm.hpp>

namespace rt
{
	class Buffer
	{
	public:
		int mSizex;
		int mSizey;
		glm::u8vec3* mData;

		Buffer(int x, int y)
		{
			mData = new glm::u8vec3[x * y];
			mSizex = x;
			mSizey = y;
		}

		~Buffer()
		{
			delete[] mData;
		}

		glm::u8vec3& Pixel(int x, int y)
		{
			return mData[x + mSizey * x];
		}
	};
}
