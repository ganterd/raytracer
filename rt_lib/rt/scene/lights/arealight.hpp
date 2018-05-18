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
		static const int mNumSamplePositionsX = 8;
		static const int mNumSamplePositionsY = 8;
		static const int mNumSamplePositions = mNumSamplePositionsX * mNumSamplePositionsY;
		glm::vec3 mSamplePositions[mNumSamplePositions];
		glm::vec3 mColourDiffuse;
		glm::vec3 mColourSpecular;
		glm::vec3 mColourAmbient;

		AreaLight()
		{
			mType = Point;
		}

		AreaLight(
			const glm::vec3& pos,
			const glm::vec3& dir,
			const glm::vec3& up,
			const glm::vec2& size,
			const glm::vec3& colourDiffuse,
			const glm::vec3& colourSpecular,
			const glm::vec3& colourAmbient
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
			float dx = mSize.x / (float)mNumSamplePositionsX;
			float dy = mSize.y / (float)mNumSamplePositionsY;
			for(int x = 0; x < mNumSamplePositionsX; ++x)
			{
				float px = mSize.x * -0.5f + (float)x * dx;
				for(int y = 0; y < mNumSamplePositionsY; ++y)
				{
					float py = mSize.y * -0.5f + (float)y * dy;
					mSamplePositions[x + y * mNumSamplePositionsX] = mPosition + left * px + mUp * py;
				}
			}
		}
	};
}
