#pragma once

#include <rt/scene/lights/light.hpp>

namespace rt
{
	class AreaLight : public Light
	{
	public:
		glm::vec3 mPosition;
		glm::vec3 mDirection;
		glm::vec3 mUp;
		glm::vec2 mSize;
		int mNumSamplePositions;
		glm::vec3* mSamplePositions;
		glm::vec3 mColourDiffuse;
		glm::vec3 mColourSpecular;
		glm::vec3 mColourAmbient;

		AreaLight()
		{
			mType = Point;
		}

		~AreaLight()
		{
			delete[] mSamplePositions;
		}

		AreaLight(
			const glm::vec3& pos,
			const glm::vec3& dir,
			const glm::vec3& up,
			const glm::vec2& size,
			const glm::vec3& colourDiffuse,
			const glm::vec3& colourSpecular,
			const glm::vec3& colourAmbient,
			int numSamplePositionsX = 8,
			int numSamplePositionsY = 8
		)
		{
			mType = Area;
			mPosition = pos;
			mDirection = dir;
			mUp = up;
			mSize = size;
			mColourDiffuse = colourDiffuse;
			mColourAmbient = colourAmbient;
			mColourSpecular = colourSpecular;

			glm::vec3 left = glm::cross(mDirection, mUp);
			mNumSamplePositions = numSamplePositionsX * numSamplePositionsY;
			mSamplePositions = new glm::vec3[mNumSamplePositions];
			std::cout << "[" << numSamplePositionsX << "," << numSamplePositionsY << "]" << std::endl;
			float dx = mSize.x / (float)numSamplePositionsX;
			float dy = mSize.y / (float)numSamplePositionsY;

			
			for(int x = 0; x < numSamplePositionsX; ++x)
			{
				float px = mSize.x * -0.5f + (float)x * dx;
				for(int y = 0; y < numSamplePositionsY; ++y)
				{
					float py = mSize.y * -0.5f + (float)y * dy;

					float rx = ((float)std::rand() / (float)RAND_MAX) * dx;
					float ry = ((float)std::rand() / (float)RAND_MAX) * dy;

					mSamplePositions[x + y * numSamplePositionsX] = mPosition + left * (px + rx) + mUp * (py + ry);
				}
			}
		}
	};
}
