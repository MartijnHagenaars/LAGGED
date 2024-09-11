#pragma once
#include "GL_Cubemap.h"
#include "Platform/Base/Renderer/Buffer.h"

namespace LAG
{
	class Skybox
	{
	public:

		static void Load();
		static void Unload();

		static void Render(Cubemap& cubemap);

	private:
		Skybox() = default;
		~Skybox() = default;

		static ArrayBuffer m_Buffer;

	};
}