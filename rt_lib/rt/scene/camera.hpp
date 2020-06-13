#pragma once

#include <rt/math/mat4x4f.hpp>
#include <rt/scene/ray.hpp>

namespace rt
{
	class camera
	{
	public:
		/* Camera position */
		float4 mPosition;

		/* Position in space the camera is looking at */
		float4 mLookAt;

		/* Direction in space that orients the camera upwards */
		float4 mUp;

		float4 mForward;
		float4 mLowerLeft;
		float4 mU;
		float4 mV;


		float mFocalLength;
		float mFrameHeight;
		float mFrameWidth;
		float mAspectRatio;
		float mHorizontalFOV;

	public:
		camera(
			const float4& position,
			const float4& lookAt,
			const float4& up,
			float aspect
		){
			mFocalLength = 1.0f;
			mAspectRatio = (aspect == 0.0f) ? 1.0f : aspect;
			mFrameHeight = 2.0f;
			mFrameWidth = mAspectRatio * mFrameHeight;

			mPosition = position;
			mLookAt = lookAt;

			mUp = unitVector(up);
			mForward = unitVector(mLookAt - mPosition);
			float4 frameRight = -cross(mUp, mForward);
			float4 frameUp = -unitVector(cross(mForward, frameRight));
			
			mU = frameRight * mFrameWidth;
			mV = frameUp * mFrameHeight;

			float4 frameCentre = point(mPosition + mForward * mFocalLength);
			mLowerLeft = point(frameCentre - (mU * 0.5f) - (mV * 0.5f));
		}

		ray ray(float u, float v)
		{
			float4 origin = point(mPosition);
			float4 direction = unitVector(point(mLowerLeft + mU * u + mV * v) - mPosition);
			return rt::ray(origin, direction);
		}
	};
}
