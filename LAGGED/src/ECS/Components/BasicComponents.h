#pragma once
#include <string>
#include "glm/vec3.hpp"

namespace LAG
{
	struct NameComponent
	{
		std::string name;
	};


	struct TransformComponent
	{
		glm::vec3 position = glm::vec3(0.f);
		glm::vec3 rotation = glm::vec3(0.f);
		glm::vec3 scale = glm::vec3(1.f);
	};
}