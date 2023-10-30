#pragma once
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/CameraComponent.h"

namespace LAG::CameraSystem
{
	static void Update(uint32_t entityID, TransformComponent& transform, CameraComponent& camera);
}