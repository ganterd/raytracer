#include <rt/scene/scene.hpp>

bool rt::Scene::fromFile(const std::string& file)
{
	Assimp::Importer importer;

	const aiScene* assimpScene = importer.ReadFile(file, aiProcess_Triangulate);

	if(!assimpScene)
	{
		std::cerr << "Couldn't read file \"" << file << "\": " << importer.GetErrorString();
		return false;
	}

	size_t totalTris = 0;

	std::cout << "Scene:" << std::endl;
	std::cout << "   |- Meshes: " << assimpScene->mNumMeshes << std::endl;
	for(int i = 0; i < assimpScene->mNumMeshes; ++i)
	{
		aiMesh** meshes = assimpScene->mMeshes;
		std::cout << "      [" << i << "] '" << meshes[i]->mName.C_Str() << "': ";
		std::cout << " v:" << meshes[i]->mNumVertices;
		totalTris += meshes[i]->mNumVertices;

		std::cout << std::endl;
	}
	std::cout << "   |- Total Tris: " << totalTris << std::endl;


	std::cout << "   |- Lights: " << assimpScene->mNumLights << std::endl;
	std::cout << "   |- Cameras: " << assimpScene->mNumCameras << std::endl;
	std::cout << "   |- Textures: " << assimpScene->mNumTextures << std::endl;

	return true;
}
