#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <rt/scene/lights/light.hpp>
#include <rt/scene/mesh/mesh.hpp>

namespace rt
{
	class Scene
	{
	public:
		std::vector<Mesh*> m_Meshes;
		std::vector<Light*> m_Lights;

		bool fromFile(const std::string& file);
	};
}
