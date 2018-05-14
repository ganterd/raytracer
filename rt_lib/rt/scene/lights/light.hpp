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
	};
}
