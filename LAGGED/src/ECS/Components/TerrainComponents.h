#pragma once
#include "Core/Resources/Texture.h"

namespace LAG
{
	struct PlaneComponent
	{
		Texture* texture = nullptr;
	};

	struct ProceduralTerrainComponent
	{
		float frequency = 0.f;
		float amplitude = 0.f;
		int seed = 0;
	};
}