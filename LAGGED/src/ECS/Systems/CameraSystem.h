#pragma once
#include "glm/mat4x4.hpp"

namespace LAG::CameraSystem
{
	static void Update(uint32_t entityID);

	static glm::mat4 CalculateViewMatrix(uint32_t entityID);
	static glm::mat4 CalculateProjMat(uint32_t entityID);
}