#include <rt/scene/scene.hpp>

bool rt::scene::fromFile(const std::string& file)
{
	Assimp::Importer importer;

	assimpScene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenNormals);

	if(!assimpScene)
	{
		std::cerr << "Couldn't read file \"" << file << "\": " << importer.GetErrorString();
		return false;
	}

	mGeometryAABB = rt::aabb::infinity();
	mCentroidsAABB = rt::aabb::infinity();

	m_TotalTris = 0;
	for(unsigned int m = 0; m < assimpScene->mNumMeshes; ++m)
		m_TotalTris += assimpScene->mMeshes[m]->mNumFaces;

	m_Tris = (rt::tri*)_mm_malloc(m_TotalTris * sizeof(rt::tri), alignof(__m128));
	m_AssignedTris = 0;

	std::cout << "Scene:" << std::endl;
	std::cout << "   |- Total Tris: " << m_TotalTris << std::endl;
	std::cout << "   |- Meshes: " << assimpScene->mNumMeshes << std::endl;
	
	aiMatrix4x4 identity;
	NodeRecurse(assimpScene->mRootNode, identity);
	
	std::cout << "   |- Centroid AABB " << mCentroidsAABB << std::endl;
	std::cout << "   |- Geometry AABB " << mGeometryAABB << std::endl;

	//ProcessLights();
	std::cout << "   |- Cameras: " << assimpScene->mNumCameras << std::endl;
	if(assimpScene->mNumCameras)
		ProcessCamera(assimpScene->mCameras[0]);
	std::cout << "   |- Textures: " << assimpScene->mNumTextures << std::endl;

	return true;
}

void rt::scene::NodeRecurse(aiNode* n, const aiMatrix4x4& t)
{
	aiMatrix4x4 currentTransform =  n->mTransformation * t;
	for(unsigned int m = 0; m < n->mNumMeshes; ++m)
	{
		ProcessMesh(assimpScene->mMeshes[n->mMeshes[m]], currentTransform);
	}

	for(unsigned int c = 0; c < n->mNumChildren; ++c)
	{
		NodeRecurse(n->mChildren[c], currentTransform);
	}
}

void rt::scene::ProcessMesh(aiMesh* m, const aiMatrix4x4& transform)
{	
	//std::cout << "      '" << m->mName.C_Str() << "': ";

	int tris = m->mNumFaces;
	//std::cout << " v:" << tris;
	for(int t = 0; t < tris; ++t)
	{
		float4 v0, v1, v2;
		float4 n0, n1, n2;
		mat4x4f mat = toNative(transform);

		/* Import and transform the vertices and normals for this triangle */
		v0 = mat * toNativePoint(m->mVertices[m->mFaces[t].mIndices[0]]);
		v1 = mat * toNativePoint(m->mVertices[m->mFaces[t].mIndices[1]]);
		v2 = mat * toNativePoint(m->mVertices[m->mFaces[t].mIndices[2]]);
		n0 = mat * toNativeVector(m->mNormals[m->mFaces[t].mIndices[0]]);
		n1 = mat * toNativeVector(m->mNormals[m->mFaces[t].mIndices[1]]);
		n2 = mat * toNativeVector(m->mNormals[m->mFaces[t].mIndices[2]]);

		rt::tri tri(v0, v1, v2, n0, n1, n2);
		m_Tris[m_AssignedTris++] = tri;

		mCentroidsAABB.grow(tri.centroid);
		mGeometryAABB.grow(v0);
		mGeometryAABB.grow(v1);
		mGeometryAABB.grow(v2);
	}
	//m_TotalTris += tris;

	//std::cout << std::endl;
}

void rt::scene::ProcessCamera(aiCamera* c)
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
	cl *= ct;
	cu *= rotationMatrix;

	mCamera = new camera(
		toNativePoint(cp), 
		toNativePoint(cl), 
		toNativeVector(cu), 
		c->mAspect//, 
		//c->mHorizontalFOV
	);
}

// void rt::scene::ProcessLights()
// {
// 	std::cout << "   |- Lights: " << assimpScene->mNumLights << std::endl;
// 	for(unsigned int i = 0; i < assimpScene->mNumLights; ++i)
// 	{
// 		aiLight* l = assimpScene->mLights[i];
// 		std::cout << "      |- '" << l->mName.C_Str() << "'" << std::endl;
		
// 		std::string lightType = "";
// 		switch(l->mType)
// 		{
// 		case aiLightSource_UNDEFINED:
// 			lightType = "Unknown";
// 			break;
// 		case aiLightSource_DIRECTIONAL:
// 			lightType = "Directional";
// 			break;
// 		case aiLightSource_POINT:
// 			lightType = "Point";
// 			break;
// 		case aiLightSource_SPOT:
// 			lightType = "Spot";
// 			break;
// 		case aiLightSource_AMBIENT:
// 			lightType = "Ambient";
// 			break;
// 		case aiLightSource_AREA:
// 			lightType = "Area";
// 			break;
// 		default:
// 			std::cerr << "Unknown light type..." << std::endl;
// 			break;
// 		}
// 		std::cout << "         |- Type: " << lightType << std::endl;

// 		// Calculate the transformations for the light's node
// 		aiNode* node = assimpScene->mRootNode->FindNode(l->mName);
// 		aiMatrix4x4 transform = node->mTransformation;
// 		node = node->mParent;
// 		while(node)
// 		{
// 			transform = node->mTransformation * transform;
// 			node = node->mParent;
// 		}
// 		aiMatrix3x3 rotation(transform);

// 		// Determine and transform positions and vectors
// 		aiVector3D pos = transform * l->mPosition;
// 		std::cout << "         |- Pos: [" << pos.x << "," << pos.y << "," << pos.z << "]" << std::endl;

// 		aiVector3D dir = transform * l->mDirection - pos;
// 		std::cout << "         |- Dir: [" << dir.x << "," << dir.y << "," << dir.z << "]" << std::endl;

// 		aiVector3D up = transform * l->mUp - pos;
// 		std::cout << "         |- Up: [" << up.x << "," << up.y << "," << up.z << "]" << std::endl;

// 		aiVector2D area = l->mSize;
// 		std::cout << "         |- Area: [" << area.x << "," << area.y << "]" << std::endl;

// 		aiColor3D diff = l->mColorDiffuse;
// 		std::cout << "         |- Diffuse: [" << diff.r << "," << diff.g << "," << diff.b << "]" << std::endl;

// 		aiColor3D spec = l->mColorSpecular;
// 		std::cout << "         |- Spec: [" << spec.r << "," << spec.g << "," << spec.b << "]" << std::endl;	

// 		aiColor3D amb = l->mColorAmbient;
// 		std::cout << "         |- Amb: [" << amb.r << "," << amb.g << "," << amb.b << "]" << std::endl;

// 		float atten0 = l->mAttenuationConstant;
// 		float atten1 = l->mAttenuationLinear;
// 		float atten2 = l->mAttenuationQuadratic;
// 		std::cout << "         |- Atten0: " << atten0 << std::endl;
// 		std::cout << "         |- Atten1: " << atten1 << std::endl;
// 		std::cout << "         |- Atten2: " << atten2 << std::endl;

// 		Light* light;
// 		if(l->mType == aiLightSource_AREA)
// 		{
// 			light = new AreaLight(
// 				glm::vec3(pos.x, pos.y, pos.z),
// 				glm::vec3(dir.x, dir.y, dir.z),
// 				glm::vec3(up.x, up.y, up.z),
// 				glm::vec2(area.x, area.y),
// 				glm::vec3(diff.r, diff.g, diff.b),
// 				glm::vec3(spec.r, spec.g, spec.b),
// 				glm::vec3(amb.r, amb.g, amb.b)
// 			);
// 		}
// 		else
// 		{
// 			light = new PointLight(
// 				glm::vec3(pos.x, pos.y, pos.z),
// 				glm::vec3(diff.r, diff.g, diff.b),
// 				glm::vec3(spec.r, spec.g, spec.b),
// 				glm::vec3(amb.r, amb.g, amb.b)
// 			);
// 		}
// 		mLights.push_back(light);
// 	}
// }