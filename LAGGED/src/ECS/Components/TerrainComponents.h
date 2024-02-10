#pragma once
#include "Core/Resources/Surface.h"
#include "Core/Resources/ResourceHandles.h"

#include "Utility/Noise.h"
#include <memory>

namespace LAG
{
	struct SurfaceComponent
	{
		SurfaceComponent();
		~SurfaceComponent();

		std::unique_ptr<Surface> m_Surface = nullptr;
		TextureHandle m_SurfaceTexture = {};

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};

	struct ProceduralSurfaceComponent
	{
		ProceduralSurfaceComponent();
		~ProceduralSurfaceComponent();

		Surface m_Surface = {};
		//std::unique_ptr<Surface> m_Surface = nullptr;
		TextureHandle m_SurfaceTexture = {};

		//TextureHandle m_SurfaceHeightmap = {};

		NoiseProperties m_NoiseProperties = {};

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};
}