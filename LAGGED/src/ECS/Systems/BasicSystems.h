#pragma once
#include <stdint.h>

namespace LAG
{
	namespace BasicSystems
	{
		void UpdateBasicSystems();
	}

	namespace SinWaveSystem
	{
		void Update(uint32_t entityID);
	}
}