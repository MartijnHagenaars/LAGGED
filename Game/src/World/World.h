#pragma once
#include <vector>
#include <unordered_map>
#include "glm/vec3.hpp"
#include "ChunkKey.h"

class Chunk;
class World
{
public:
	explicit World(int loadDistance);
	~World();

	void Update();

private:
	std::unordered_map<ChunkKey, Chunk> m_ChunkMap;

	const int m_LoadDistance = 0;

	glm::vec3 m_PrevCameraPos = glm::vec3(0.f);
};

