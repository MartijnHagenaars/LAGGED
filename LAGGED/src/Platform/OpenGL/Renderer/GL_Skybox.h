#pragma once
#include "GL_Cubemap.h"
#include "Platform/Base/Renderer/Buffer.h"

namespace LAG
{
	class Skybox
	{
	public:
		Skybox() = default;
		~Skybox() = default;

		static void Load();
		static void Unload();

		static void Render(Cubemap& cubemap);

	private:
		inline static ArrayBuffer m_Buffer;

	};
}