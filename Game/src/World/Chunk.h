#pragma once
#include "glm/vec2.hpp"
#include "ECS/Entity.h"

static const int CHUNK_SIZE = 64;

class Chunk
{
public: 
	void Load(const glm::vec2& position);
	void Unload();

private:
	LAG::Entity m_Entity;
};

