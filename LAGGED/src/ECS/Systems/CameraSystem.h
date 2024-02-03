#pragma once
#include "glm/glm.hpp"
#include "ECS/Entity.h"

namespace LAG::CameraSystem
{
	void Update(Entity* entity);

	glm::mat4 CalculateViewMat(Entity* entity);
	glm::mat4 CalculateProjMat(Entity* entity);
}