#pragma once
#include "glm/vec2.hpp"
#include "ECS/Entity.h"

static const int CHUNK_SIZE = 64;

namespace LAG
{
	class Entity;
}

class Chunk
{
public: 
	Chunk() = default;
	~Chunk() = default;

	void Load(const glm::vec2& position);
	void Unload();

private:
	LAG::Entity m_Entity;
};

