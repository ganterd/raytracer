#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <rt/scene/lights/light.hpp>
#include <rt/scene/mesh/mesh.hpp>
#include <rt/scene/mesh/tri.hpp>

namespace rt
{
	class Scene
	{
	public:
		const aiScene* assimpScene;

		std::vector<Mesh*> m_Meshes;
		std::vector<Tri> m_Tris;
		std::vector<Light*> m_Lights;

		size_t m_TotalTris;

		bool fromFile(const std::string& file);

	private:
		void NodeRecurse(aiNode* n, const aiMatrix4x4& m);
		void ProcessMesh(aiMesh*, const aiMatrix4x4& m);
	};
}
