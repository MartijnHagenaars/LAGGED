#pragma once
#include "ECS/Entity.h"
#include "Core/Resources/Texture.h"

namespace LAG::SurfaceSystems
{
	//Generates a flat plane using two vertices
	void GenerateFlatPlane(Entity* entity);

	void GenerateNoiseSurface(Entity& entity);

	//Apply a height map to the plane
	void SetHeightMapTexture(Entity* entity, Texture* texture);
}
