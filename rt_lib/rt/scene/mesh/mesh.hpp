#pragma once

#include <rt/scene/aabb.hpp>

namespace rt
{
	class mesh
	{
	public:
		rt::aabb mAABB;
		float4* m_Verts;
		size_t m_NumVerts;
	};
}
