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
		float movementSpeed = 1.f;

		bool hasCameraMoved = true;
		bool hasCameraDimensionChanged = true;

		glm::mat4 viewMat = glm::mat4(1.f);
		glm::mat4 projMat = glm::mat4(1.f);
		CameraPerspectiveType perspectiveType = CameraPerspectiveType::PERSPECTIVE;
	};
}