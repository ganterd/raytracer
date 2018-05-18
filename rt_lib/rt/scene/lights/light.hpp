#pragma once

#include <glm/glm.hpp>

namespace rt
{
	class Light
	{
	public:
		enum Type
		{
			Directional,
			Point,
			Spot,
			Ambient,
			Area
		};

		Type mType;

		/**
 		 * Calculate the intensity of the light at a distance d
 		 */
		float intensity(float d)
		{
			return 1.0f / (d * d);
			//return 1.0f / (mAttenuationLinear + mAttenuationConstant * d + mAttenuationQuadratic * d * d);
		}
	};
}
