#pragma once
#include "glm/vec2.hpp"

namespace LAG
{
	class Terrain
	{
	public:
		Terrain(); 
		~Terrain(); 

		void SetTerrainProperties(const glm::vec2& size, const glm::vec2& subdiv);

		void Render();
	};
}
