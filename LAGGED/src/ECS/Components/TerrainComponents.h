#pragma once
#include "Core/Resources/Surface.h"
#include "Core/Resources/ResourceHandles.h"

#include "Utility/Noise.h"
#include <memory>

namespace LAG
{
	struct SurfaceComponent
	{
		std::unique_ptr<Surface> surface = nullptr;
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