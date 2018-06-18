
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <rt/scene/lights/light.hpp>
#include <rt/scene/lights/pointlight.hpp>
#include <rt/scene/lights/arealight.hpp>
#include <rt/scene/mesh/mesh.hpp>
#include <rt/scene/mesh/tri.hpp>
#include <rt/scene/camera.hpp>

namespace rt
{
	class Scene
	{
	public:
		const aiScene* assimpScene;

		std::vector<Mesh*> m_Meshes;
		//std::vector<Tri> m_Tris;
		std::vector<Light*> mLights;

		AABB mGeometryAABB;
		AABB mCentroidsAABB;

		Camera* mCamera;

		size_t m_TotalTris;
		rt::Tri* m_Tris;
		size_t m_AssignedTris;

		bool fromFile(const std::string& file);

	private:
		void NodeRecurse(aiNode* n, const aiMatrix4x4& m);
		void ProcessMesh(aiMesh*, const aiMatrix4x4& m);
		void ProcessCamera(aiCamera* c);
		void ProcessLights();
		
		glm::mat4 AssimpToGLM(const aiMatrix4x4& from)
		{
			glm::mat4 to;
			to[0][0] = from.a1; 
			to[1][0] = from.a2;
			to[2][0] = from.a3; 
			to[3][0] = from.a4;
			to[0][1] = from.b1; 
			to[1][1] = from.b2;
			to[2][1] = from.b3; 
			to[3][1] = from.b4;
			to[0][2] = from.c1; 
			to[1][2] = from.c2;
			to[2][2] = from.c3; 
			to[3][2] = from.c4;
			to[0][3] = from.d1; 
			to[1][3] = from.d2;
			to[2][3] = from.d3; 
			to[3][3] = from.d4;
			return to;
		}

		glm::vec3 AssimpToGLM(const aiVector3D& from)
		{
			return glm::vec3(from.x, from.y, from.z);
		}
	};
}
