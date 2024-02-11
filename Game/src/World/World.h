#pragma once
#include <vector>
#include "glm/vec3.hpp"

namespace LAG
{
	class Entity;
}

class World
{
public:
	explicit World(int loadDistance);
	~World();

	void Update();

private:
	std::vector<LAG::Entity*> m_Chunks;
	const int m_LoadDistance = 0;

	glm::vec3 m_PrevCameraPos = glm::vec3(0.f);
};

