#pragma once

#include <rt/scene/lights/light.hpp>
#include <xmmintrin.h>
#include <immintrin.h>

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

		__m128 mSSEPosition;
		__m128 mSSEUp;
		__m128 mSSELeft;
		glm::vec2 mHalfSize;
		glm::vec2 mSampleScaler;

		AreaLight(
			const glm::vec3& pos,
			const glm::vec3& dir,
			const glm::vec3& up,
			const glm::vec2& size,
			const glm::vec3& colourDiffuse,
			const glm::vec3& colourSpecular,
			const glm::vec3& colourAmbient,
			int numSamplePositionsX = 4,
			int numSamplePositionsY = 4
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

			mSSEPosition[0] = pos.x;
			mSSEPosition[1] = pos.y;
			mSSEPosition[2] = pos.z;
			mSSEPosition[3] = 0.0f;

			mSSEUp[0] = up.x;
			mSSEUp[1] = up.y;
			mSSEUp[2] = up.z;
			mSSEUp[3] = 0.0f;

			mSSELeft[0] = mLeft.x;
			mSSELeft[1] = mLeft.y;
			mSSELeft[2] = mLeft.z;
			mSSELeft[3] = 0.0f;

			mHalfSize = mSize * 0.5f;
			mSampleScaler = mSampleDivision * (1.0f / (float)RAND_MAX);
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
			float rx = (float)std::rand() * mSampleScaler.x - mHalfSize.x + mSampleDivision.x * (float)x;
			float ry = (float)std::rand() * mSampleScaler.y - mHalfSize.y + mSampleDivision.y * (float)y;
			__m128 SSErx = _mm_set1_ps(rx);
			__m128 SSEry = _mm_set1_ps(ry);
			__m128 r = _mm_fmadd_ps(mSSELeft, SSErx, mSSEPosition);
			r = _mm_fmadd_ps(mSSEUp, SSEry, r);
			return glm::vec3(r[0], r[1], r[2]);
		}

		__m128 sampleSSE(int i)
		{
			int x = i % mNumSamplePositionsX;
			int y = i / mNumSamplePositionsY;
			float rx = (float)std::rand() * mSampleScaler.x - mHalfSize.x + mSampleDivision.x * (float)x;
			float ry = (float)std::rand() * mSampleScaler.y - mHalfSize.y + mSampleDivision.y * (float)y;
			__m128 SSErx = _mm_set1_ps(rx);
			__m128 SSEry = _mm_set1_ps(ry);
			__m128 r = _mm_fmadd_ps(mSSELeft, SSErx, mSSEPosition);
			return _mm_fmadd_ps(mSSEUp, SSEry, r);
		}
	};
}
