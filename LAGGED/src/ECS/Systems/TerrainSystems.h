#pragma once
#include "Core/Resources/Texture.h"

namespace LAG::TerrainSystems
{
	//Generates a flat plane using two vertices
	void GenerateFlatPlane(uint32_t entityID);

	//Generates a flat plane with an amount of subdivisions
	void GenerateFlatPlane(uint32_t entityID, float subdivision);

	//Generates a flat plane using subdivisions with different sub scales for the plane width and height
	void GenerateFlatPlane(uint32_t entityID, float xSubSize, float ySubSize);

	void UseTextureAsHeightMap(uint32_t entityID, Texture* texture);
}
