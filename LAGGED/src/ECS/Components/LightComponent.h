#pragma once
#include "glm/vec3.hpp"

namespace LAG
{
	struct LightComponent
	{
		glm::vec3 lightColor = glm::vec3(1.f);
		float lightIntensity = 0.f;
		float lightAttenuation = 0.f;

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};
}