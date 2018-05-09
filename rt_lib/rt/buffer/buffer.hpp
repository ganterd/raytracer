#include <glm/glm.hpp>

namespace rt
{
	class Buffer
	{
	public:
		int mSizex;
		int mSizey;
		glm::vec3* mData;

		Buffer(int x, int y)
		{
			mData = new glm::vec3[x * y];
			mSizex = x;
			mSizey = y;
		}

		~Buffer()
		{
			delete[] mData;
		}

		glm::vec3& Pixel(int x, int y)
		{
			return mData[x + mSizey * x];
		}
	};
}
