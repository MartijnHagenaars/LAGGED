#pragma once
#include "Core/Resources/ResourceHandles.h"

namespace LAG
{
	struct ModelComponent
	{
		ModelComponent() = default;
		ModelComponent(const std::string& path) :
			modelHandle({ HashedString(path) })
		{
		}

		ModelHandle modelHandle;

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};
}