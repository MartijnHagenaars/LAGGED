#pragma once
#include "glm/vec3.hpp"

namespace LAG
{
	struct LightComponent
	{
		glm::vec3 lightColor = glm::vec3(1.f);
		float lightIntensity = 0.f;
		float lightAttenuation = 0.f;

		static int InitReflection();
		static inline int m_ReflectionID = InitReflection();
	};
}