#pragma once
#include "glm/vec3.hpp"

namespace LAG
{
	struct LightComponent
	{
		glm::vec3 lightColor = glm::vec3(1.f);
		float lightIntensity = 1.f;
	};
}