#pragma once
#include "ECS/Entity.h"
#include "Core/Resources/Texture.h"

namespace LAG::TerrainSystems
{
	//Generates a flat plane using two vertices
	void GenerateFlatPlane(Entity* entity);

	//Generates a flat plane with an amount of subdivisions
	void GenerateFlatPlane(Entity* entity, float subdivision);

	//Apply a height map to the plane
	void SetHeightMapTexture(Entity* entity, Texture* texture);
}
