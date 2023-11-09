#pragma once
#include "glm/mat4x4.hpp"

namespace LAG
{
	enum class CameraPerspectiveType
	{
		PERSPECTIVE = 0,
		ORTHOGRAPHIC
	};

	struct CameraComponent
	{
		bool isActive = true;

		float fov = 90.f;
		float movementSpeed = 5.f;
		float rotationSpeed = 5.f;

		bool hasCameraMoved = true;
		bool hasCameraDimensionChanged = true;

		glm::vec3 forwardVector = glm::vec3(0.f);
		glm::vec3 rightVector = glm::vec3(0.f);

		glm::mat4 viewMat = glm::mat4(1.f);
		glm::mat4 projMat = glm::mat4(1.f);
		CameraPerspectiveType perspectiveType = CameraPerspectiveType::PERSPECTIVE;
	};
}