#pragma once
#include <glm/mat4x4.hpp>
#include <memory>

#include "Platform/Resources/Buffers/FrameBuffer.h"

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
		
		//FIXME: Potentially really dangerous... Should we maybe store rendering objects (like frame buffers) in some sort of rendering resource manager?
		std::unique_ptr<FrameBuffer> frameBuffer = std::make_unique<FrameBuffer>();

	private:
		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};
}