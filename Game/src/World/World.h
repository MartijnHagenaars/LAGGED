#pragma once
#include <vector>
#include <unordered_map>
#include "glm/vec3.hpp"

struct ChunkKey
{
	ChunkKey(int x, int y) : 
		m_X(x), m_Y(y) 
	{}

	int m_X, m_Y;
};

//Implement std::hash functionality for ChunkKey struct.
//Required for use in a map
namespace std {
	template <> struct hash<ChunkKey> {
		size_t operator()(const ChunkKey& value) const noexcept
		{
			size_t xHash = std::hash<int>{}(value.m_X);
			size_t yHash = std::hash<int>{}(value.m_Y);

			return std::hash<size_t>{}((xHash ^ yHash) >> 2);
		}
	};
}


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

