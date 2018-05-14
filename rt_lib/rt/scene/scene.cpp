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

	ProcessLights();
	std::cout << "   |- Cameras: " << assimpScene->mNumCameras << std::endl;
	if(assimpScene->mNumCameras)
		ProcessCamera(assimpScene->mCameras[0]);
	std::cout << "   |- Textures: " << assimpScene->mNumTextures << std::endl;

	return true;
}

void rt::Scene::NodeRecurse(aiNode* n, const aiMatrix4x4& t)
{
	aiMatrix4x4 currentTransform =  n->mTransformation * t;
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

	int tris = m->mNumFaces;
	std::cout << " v:" << tris;
	for(int t = 0; t < tris; ++t)
	{
		aiVector3D v0, v1, v2;
		aiVector3D n0, n1, n2;

		v0 = transform * m->mVertices[m->mFaces[t].mIndices[0]];
		v1 = transform * m->mVertices[m->mFaces[t].mIndices[1]];
		v2 = transform * m->mVertices[m->mFaces[t].mIndices[2]];
		n0 = transform * m->mNormals[m->mFaces[t].mIndices[0]];
		n1 = transform * m->mNormals[m->mFaces[t].mIndices[1]];
		n2 = transform * m->mNormals[m->mFaces[t].mIndices[2]];

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

	aiMatrix3x3 rotationMatrix(ct);

	cp *= ct;
	cl *= ct;//rotationMatrix;
	cu *= rotationMatrix;

//	mCamera = new Camera(
//		glm::vec3(-20,20,20),//glm::vec3(cp.x, cp.y, cp.z), 
//		glm::vec3(0,0,0),//glm::vec3(cl.x, cl.y, cl.z), 
//		glm::vec3(0,0,1),//glm::vec3(cu.x, cu.y, cu.z), 
//		c->mAspect, 
//		glm::degrees(c->mHorizontalFOV)
//	);


	mCamera = new Camera(
		glm::vec3(cp.x, cp.y, cp.z), 
		glm::vec3(cl.x, cl.y, cl.z), 
		glm::vec3(cu.x, cu.y, cu.z), 
		c->mAspect, 
		c->mHorizontalFOV
	);
}

void rt::Scene::ProcessLights()
{
	std::cout << "   |- Lights: " << assimpScene->mNumLights << std::endl;
	for(int i = 0; i < assimpScene->mNumLights; ++i)
	{
		aiLight* l = assimpScene->mLights[i];
		std::cout << "      |- '" << l->mName.C_Str() << "'" << std::endl;
		
		std::string lightType = "";
		switch(l->mType)
		{
		case aiLightSource_UNDEFINED:
			lightType = "Unknown";
			break;
		case aiLightSource_DIRECTIONAL:
			lightType = "Directional";
			break;
		case aiLightSource_POINT:
			lightType = "Point";
			break;
		case aiLightSource_SPOT:
			lightType = "Spot";
			break;
		case aiLightSource_AMBIENT:
			lightType = "Ambient";
			break;
		case aiLightSource_AREA:
			lightType = "Area";
			break;
		}
		std::cout << "         |- Type: " << lightType << std::endl;

		// Calculate the transformations for the light's node
		aiNode* node = assimpScene->mRootNode->FindNode(l->mName);
		aiMatrix4x4 transform = node->mTransformation;
		node = node->mParent;
		while(node)
		{
			transform = node->mTransformation * transform;
			node = node->mParent;
		}
		aiMatrix3x3 rotation(transform);

		// Determine and transform positions and vectors
		aiVector3D pos = transform * l->mPosition;
		std::cout << "         |- Pos: [" << pos.x << "," << pos.y << "," << pos.z << "]" << std::endl;

		aiVector3D dir = transform * l->mDirection - pos;
		std::cout << "         |- Dir: [" << dir.x << "," << dir.y << "," << dir.z << "]" << std::endl;

		aiVector3D up = transform * l->mUp - pos;
		std::cout << "         |- Up: [" << up.x << "," << up.y << "," << up.z << "]" << std::endl;

		aiVector2D area = l->mSize;
		std::cout << "         |- Area: [" << area.x << "," << area.y << "]" << std::endl;

		aiColor3D diff = l->mColorDiffuse;
		std::cout << "         |- Diffuse: [" << diff.r << "," << diff.g << "," << diff.b << "]" << std::endl;

		aiColor3D spec = l->mColorSpecular;
		std::cout << "         |- Spec: [" << spec.r << "," << spec.g << "," << spec.b << "]" << std::endl;	

		aiColor3D amb = l->mColorAmbient;
		std::cout << "         |- Amb: [" << amb.r << "," << amb.g << "," << amb.b << "]" << std::endl;

		float atten0 = l->mAttenuationConstant;
		float atten1 = l->mAttenuationLinear;
		float atten2 = l->mAttenuationQuadratic;
		std::cout << "         |- Atten0: " << atten0 << std::endl;
		std::cout << "         |- Atten1: " << atten1 << std::endl;
		std::cout << "         |- Atten2: " << atten2 << std::endl;

		// TMP: For now everything is a point light;
		PointLight* light = new PointLight(
			glm::vec3(pos.x, pos.y, pos.z),
			glm::vec3(diff.r, diff.g, diff.b),
			glm::vec3(spec.r, spec.g, spec.b),
			glm::vec3(amb.r, amb.g, amb.b)
		);
		light->mAttenuationConstant = atten0;
		light->mAttenuationLinear = atten1;
		light->mAttenuationQuadratic = atten2;
		mLights.push_back(light);
	}
}
