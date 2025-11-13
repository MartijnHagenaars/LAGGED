#pragma once
#include <glm/mat4x4.hpp>

#include "ECS/TypeDefines.h"

namespace LAG
{
	namespace CameraSystem
	{
		void Update(EntityID entityID);

		EntityID GetActiveCameraEntityID();

		glm::mat4 CalculateViewMat(EntityID entityID);
		glm::mat4 CalculateProjMat(EntityID entityID);
	}
}