#pragma once
#include "glm/glm.hpp"

namespace LAG::CameraSystem
{
	void Update(uint32_t entityID);

	glm::mat4 CalculateViewMat(uint32_t entityID);
	glm::mat4 CalculateProjMat(uint32_t entityID);

	void DrawEditorWindow();
}