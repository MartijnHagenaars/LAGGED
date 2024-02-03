#pragma once
#include "Core/Resources/Texture.h"
#include "Core/Resources/Surface.h"

namespace LAG
{
	struct PlaneComponent
	{
		Surface* m_Surface = nullptr;
		Texture* m_Texture = nullptr;

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};

	struct ProceduralTerrainComponent
	{
		Surface* m_Surface = nullptr;
		Texture* texture = nullptr;
		Texture* heightmap = nullptr;

		float m_Frequency = 0.f;
		float m_Amplitude = 0.f;
		int m_Seed = 0;
	};
}