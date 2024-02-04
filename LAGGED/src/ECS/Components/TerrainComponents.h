#pragma once
#include "Core/Resources/Surface.h"
#include "Core/Resources/ResourceHandles.h"

namespace LAG
{
	struct SurfaceComponent
	{
		Surface* m_Surface = nullptr;
		TextureHandle m_SurfaceTexture = {};

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};

	struct ProceduralTerrainComponent
	{
		Surface* m_Surface = nullptr;
		TextureHandle m_SurfaceTexture = {};
		TextureHandle m_SurfaceHeightmap = {};

		float m_Frequency = 0.f;
		float m_Amplitude = 0.f;
		int m_Seed = 0;
	};
}