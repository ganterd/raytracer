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
		glm::vec3 mLeft;
		glm::vec2 mSize;
		glm::vec2 mSampleDivision;
		int mNumSamplePositionsX;
		int mNumSamplePositionsY;
		int mNumSamplePositions;
		glm::vec3 mColourDiffuse;
		glm::vec3 mColourSpecular;
		glm::vec3 mColourAmbient;

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
			mLeft = glm::normalize(glm::cross(mDirection, mUp));
			mSize = size;
			mColourDiffuse = colourDiffuse;
			mColourAmbient = colourAmbient;
			mColourSpecular = colourSpecular;
			mNumSamplePositionsX = numSamplePositionsX;
			mNumSamplePositionsY = numSamplePositionsY;
			mNumSamplePositions = numSamplePositionsX * numSamplePositionsY;
			mSampleDivision.x = mSize.x / (float)mNumSamplePositionsX;
			mSampleDivision.y = mSize.y / (float)mNumSamplePositionsY;
		}

		glm::vec3 sample()
		{
			float rx = ((float)std::rand() / (float)RAND_MAX) * mSize.x - mSize.x * 0.5f;
			float ry = ((float)std::rand() / (float)RAND_MAX) * mSize.y - mSize.y * 0.5f;
			return mPosition + mLeft * rx + mUp * ry;
		}

		glm::vec3 sample(int i)
		{
			int x = i % mNumSamplePositionsX;
			int y = i / mNumSamplePositionsY;
			float rx = ((float)std::rand() / (float)RAND_MAX) * mSampleDivision.x - mSize.x * 0.5f + mSampleDivision.x * (float)x;
			float ry = ((float)std::rand() / (float)RAND_MAX) * mSampleDivision.y - mSize.y * 0.5f + mSampleDivision.y * (float)y;
			return mPosition + mLeft * rx + mUp * ry;
		}
	};
}
