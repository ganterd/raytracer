#pragma once

#include <rt/scene/lights/light.hpp>

namespace rt
{
	class PointLight : public Light
	{
	public:
		glm::vec3 mPosition;
		glm::vec3 mColourDiffuse;
		glm::vec3 mColourSpecular;
		glm::vec3 mColourAmbient;

		float mAttenuationConstant;
		float mAttenuationLinear;
		float mAttenuationQuadratic;

		PointLight()
		{
			mType = Point;
		}

		PointLight(
			const glm::vec3& pos,
			const glm::vec3& colourDiffuse,
			const glm::vec3& colourSpecular,
			const glm::vec3& colourAmbient
		)
		{
			mType = Point;
			mPosition = pos;
			mColourDiffuse = colourDiffuse;
			mColourAmbient = colourAmbient;
			mColourSpecular = colourSpecular;
		}

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
