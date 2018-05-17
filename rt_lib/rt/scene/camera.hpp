#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <rt/scene/ray.hpp>

namespace rt
{
	class Camera
	{
	public:
		glm::vec3 mLookAt;
		glm::vec3 mUp;
		glm::vec3 mPosition;
		glm::mat4 mLookAtMatrix;
		glm::mat4 mLookAtInvMatrix;
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
			mUp = glm::normalize(u);
			if(aspect == 0.0f)
				mAspectRatio = 1.0f;
			else
				mAspectRatio = aspect;
			mHFOV = fov;
			//mHFOV = glm::radians(25.0f);

			std::cout << "Camera Instance: " << std::endl;
			std::cout << "|-p[" << mPosition.x << "," << mPosition.y << "," << mPosition.z << "]" <<std::endl;
			std::cout << "|-l[" << mLookAt.x << "," << mLookAt.y << "," << mLookAt.z << "]" << std::endl;
			std::cout << "|-u[" << mUp.x << "," << mUp.y << "," << mUp.z << "]" << std::endl;
			std::cout << "|-f[" << mHFOV << " rads]" << std::endl;

			mLookAtMatrix = glm::lookAt(mPosition, mLookAt, mUp);
			mLookAtInvMatrix = glm::inverse(mLookAtMatrix);
			mProjectionMatrix = glm::infinitePerspective(mHFOV, mAspectRatio, 0.01f);
		}
			

		rt::Ray Ray(int x, int y, int vpx, int vpy)
		{
			glm::vec3 p = glm::unProject(
				glm::vec3(x, y, 0.001f),
				mLookAtMatrix,
				mProjectionMatrix,
				glm::vec4(0.0f, 0.0f, vpx, vpy)
			);

			//p = glm::vec3(glm::vec4(p.x, p.y, p.z, 0.0f) * mLookAtInvMatrix);
			
			return rt::Ray(mPosition, glm::normalize(p - mPosition));
		}

		void SetAspectRatio(float a)
		{
			mAspectRatio = a;
			mProjectionMatrix = glm::infinitePerspective(mHFOV, mAspectRatio, 0.01f);
		}
	};
}
