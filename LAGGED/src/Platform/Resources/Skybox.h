#pragma once
#include "Platform/Resources/Cubemap.h"
#include "Platform/Resources/Buffers/Buffers.h"
#include "ECS/TypeDefines.h"

namespace LAG
{
	class Skybox
	{
	public:
		static void Load();
		static void Unload();

		static void SetCubemap(const HashedString& path);

		static void Render(EntityID camEntityID);

	private:
		inline static ArrayBuffer m_Buffer;
		inline static Cubemap* m_Cubemap = nullptr;

	};
}