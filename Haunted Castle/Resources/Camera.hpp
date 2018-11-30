#pragma once

#include "SceneObject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "CameraPoint.hpp"
#include "Const.hpp"

namespace cgue {
	class Camera : public SceneObject
	{
	public:
		Camera();
		virtual ~Camera();

		mat4x4 getCameraModel();
		mat4x4 getInverseCameraModel();
		vec3 getCameraPos();
		vec3 getCameraUp();
		vec3 getCameraLookAt();
		void advance(float tf);
		bool getAutomaticCameraMovementActivated();
		void changeAutomaticCameraMovementActivatedState();
	private:
		float t = 0;

		float automaticCameraMovementActivated = true;

		vec3 p, d;
	};
}