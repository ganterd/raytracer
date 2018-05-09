#include <glm/glm.hpp>

namespace rt
{
	class Mesh
	{
	public:
		glm::vec3 m_BoundsMin;
		glm::vec3 m_BoundsMax;

		glm::vec3* m_Verts;
	};
}
