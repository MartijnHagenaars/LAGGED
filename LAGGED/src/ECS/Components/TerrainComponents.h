#pragma once
#include "Core/Resources/Surface.h"
#include "Core/Resources/ResourceHandles.h"

#include "Utility/Noise.h"

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

		//TextureHandle m_SurfaceHeightmap = {};

		NoiseData m_NoiseData = {};

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};
}