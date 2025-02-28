#pragma once
#include "glm/glm.hpp"
#include "ECS/Entity.h"
#include "ECS/Components/CameraComponent.h"

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