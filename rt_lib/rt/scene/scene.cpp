#include <rt/scene/scene.hpp>

bool rt::Scene::fromFile(const std::string& file)
{
	Assimp::Importer importer;

	assimpScene = importer.ReadFile(file, aiProcess_Triangulate);

	if(!assimpScene)
	{
		std::cerr << "Couldn't read file \"" << file << "\": " << importer.GetErrorString();
		return false;
	}

	m_TotalTris = 0;

	std::cout << "Scene:" << std::endl;
	std::cout << "   |- Meshes: " << assimpScene->mNumMeshes << std::endl;
	
	aiMatrix4x4 identity;
	NodeRecurse(assimpScene->mRootNode, identity);

	std::cout << "   |- Total Tris: " << m_TotalTris << std::endl;


	std::cout << "   |- Lights: " << assimpScene->mNumLights << std::endl;
	std::cout << "   |- Cameras: " << assimpScene->mNumCameras << std::endl;
	if(assimpScene->mNumCameras)
		ProcessCamera(assimpScene->mCameras[0]);
	std::cout << "   |- Textures: " << assimpScene->mNumTextures << std::endl;

	return true;
}

void rt::Scene::NodeRecurse(aiNode* n, const aiMatrix4x4& t)
{
	aiMatrix4x4 currentTransform = t * n->mTransformation;
	for(int m = 0; m < n->mNumMeshes; ++m)
	{
		ProcessMesh(assimpScene->mMeshes[n->mMeshes[m]], currentTransform);
	}

	for(int c = 0; c < n->mNumChildren; ++c)
	{
		NodeRecurse(n->mChildren[c], currentTransform);
	}
}

void rt::Scene::ProcessMesh(aiMesh* m, const aiMatrix4x4& transform)
{	
	std::cout << "      '" << m->mName.C_Str() << "': ";

	int tris = m->mNumVertices;
	std::cout << " v:" << tris;
	for(int t = 0; t < tris; ++t)
	{
		aiVector3D v0, v1, v2;
		aiVector3D n0, n1, n2;

		v0 = transform * m->mVertices[t + 0];
		v1 = transform * m->mVertices[t + 1];
		v2 = transform * m->mVertices[t + 2];
		n0 = transform * m->mNormals[t + 0];
		n1 = transform * m->mNormals[t + 1];
		n2 = transform * m->mNormals[t + 2];

		m_Tris.push_back(Tri(
			glm::vec3(v0.x, v0.y, v0.z), 
			glm::vec3(v1.x, v1.y, v1.z), 
			glm::vec3(v2.x, v2.y, v2.z), 
			glm::vec3(n0.x, n0.y, n0.z), 
			glm::vec3(n1.x, n1.y, n1.z), 
			glm::vec3(n2.x, n2.y, n2.z)
		));	
	}
	m_TotalTris += tris;

	std::cout << std::endl;
}

void rt::Scene::ProcessCamera(aiCamera* c)
{

	aiVector3D cp = c->mPosition;
	aiVector3D cl = c->mLookAt;
	aiVector3D cu = c->mUp;

	aiNode* cn = assimpScene->mRootNode->FindNode(c->mName);

	aiMatrix4x4 ct = cn->mTransformation;

	cn = cn->mParent;
	while(cn->mParent)
	{
		ct = cn->mTransformation * ct;
		cn = cn->mParent;
	}

	cp *= ct;
	cl *= ct;
	cu *= ct;

	mCamera = new Camera(
		glm::vec3(cp.x, cp.y, cp.z), 
		glm::vec3(cl.x, cl.y, cl.z), 
		glm::vec3(cu.x, cu.y, cu.z), 
		c->mAspect, 
		c->mHorizontalFOV
	);
}
