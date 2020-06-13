
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include <rt/scene/lights/light.hpp>
//#include <rt/scene/lights/pointlight.hpp>
//#include <rt/scene/lights/arealight.hpp>
#include <rt/scene/mesh/mesh.hpp>
#include <rt/scene/tri.hpp>
#include <rt/scene/camera.hpp>

namespace rt
{
	class scene
	{
	public:
		const aiScene* assimpScene;

		std::vector<mesh*> m_Meshes;
		//std::vector<Tri> m_Tris;
		//std::vector<Light*> mLights;

		rt::aabb mGeometryAABB;
		rt::aabb mCentroidsAABB;

		rt::camera* mCamera;

		size_t m_TotalTris;
		rt::tri* m_Tris;
		size_t m_AssignedTris;

		bool fromFile(const std::string& file);

	private:
		void NodeRecurse(aiNode* n, const aiMatrix4x4& m);
		void ProcessMesh(aiMesh*, const aiMatrix4x4& m);
		void ProcessCamera(aiCamera* c);
		//void ProcessLights();
		
		mat4x4f toNative(const aiMatrix4x4& from)
		{
			mat4x4f to;
			for(int i = 0; i < 4; ++i)
			{
				to.rows[i].x = from[i][0];
				to.rows[i].y = from[i][1];
				to.rows[i].z = from[i][2];
				to.rows[i].w = from[i][3];
			}
			return to;
		}

		float4 toNativePoint(const aiVector3D& from)
		{
			return float4(from.x, from.y, from.z, 1.0f);
		}

		float4 toNativeVector(const aiVector3D& from)
		{
			return float4(from.x, from.y, from.z, 0.0f);
		}
	};
}
