#pragma once
#include "ECS/Entity.h"

namespace LAG
{
	namespace BasicSystems
	{
		void UpdateBasicSystems();
	}

	namespace SinWaveSystem
	{
		void Update(Entity* entity);
	}
}