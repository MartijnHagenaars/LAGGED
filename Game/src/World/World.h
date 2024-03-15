#pragma once
#include <vector>
#include <unordered_map>
#include "glm/vec3.hpp"
#include "ChunkKey.h"

class Chunk;
class World
{
public:
	explicit World(int loadDistance, bool infiniteTerrain);
	~World();

	void Update();

private:
	std::unordered_map<ChunkKey, Chunk> m_ChunkMap;

	const int m_LoadDistance = 0;

	bool m_UseInfiniteTerrain = true;
	bool m_HasGeneratedTerrain = false;

	bool m_UpdateChunks = false;
	glm::vec3 m_PrevCameraPos = glm::vec3(0.f);
};

