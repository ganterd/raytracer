#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace rt
{
	class Camera
	{
	public:
		glm::vec3 mLookAt;
		glm::vec3 mUp;
		glm::vec3 mPosition;
		glm::mat4 mLookAtMatrix;
		glm::mat4 mProjectionMatrix;
		float mAspectRatio;
		float mHFOV;

		Camera(
			const glm::vec3& p, 
			const glm::vec3& l, 
			const glm::vec3& u,
			float aspect,
			float fov
		){
			mPosition = p;
			mLookAt = l;
			mUp = u;
			if(aspect == 0.0f)
				mAspectRatio = 1.0f;
			else
				mAspectRatio = aspect;
			mHFOV = glm::degrees(fov);	
			mLookAtMatrix = glm::lookAt(mPosition, mLookAt, mUp);
			mProjectionMatrix = glm::perspective(mHFOV, mAspectRatio, 0.001f, 10000.0f);
		}
			

		glm::vec3 Ray(int x, int y, int vpx, int vpy)
		{
			return glm::normalize(glm::unProject(
				glm::vec3(x, y, 0.1f),
				mLookAtMatrix,
				mProjectionMatrix,
				glm::vec4(0.0f, 0.0f, vpx, vpy)
			));
		}
	};
}
