#pragma once

#include "Core/Resources/ResourceHandles.h"
#include "Platform/RenderBackend.h"

#include "Utility/Noise.h"

namespace LAG
{
	struct SurfaceComponent
	{
		Surface surface = {};
		TextureHandle surfaceTexture = {};

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};

	struct ProceduralSurfaceComponent
	{
		Surface surface = {};
		int surfaceSubdivisions = 16;
		glm::vec2 surfaceScale = glm::vec2(1.f);

		TextureHandle surfaceTexture = {};

		Noise::Properties noiseProperties = {};

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};
}