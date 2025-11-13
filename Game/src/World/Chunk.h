#pragma once
#include "glm/vec2.hpp"
#include "ECS/TypeDefines.h"

static const int CHUNK_SIZE = 64;

class Chunk
{
public: 
	void Load(const glm::vec2& position);
	void Unload();

private:
	LAG::EntityID m_EntityID;
};

