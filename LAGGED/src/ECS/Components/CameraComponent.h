#pragma once

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

		CameraPerspectiveType perspectiveType = CameraPerspectiveType::PERSPECTIVE;
	};
}