#pragma once
#include "Core/Resources/Resource.h"
#include "Core/Defines.h"

namespace LAG
{
	class TextureBase : public Resource
	{
	public:
		TextureBase() = delete;
		explicit TextureBase(const HashedString& path) : Resource(path) {};
		virtual ~TextureBase() {};

		virtual void Bind(size_t textureUnit) = 0;
		virtual void Unbind(size_t textureUnit) = 0;

	protected:
		int m_TexWidth = 0;
		int m_TexHeight = 0;
		int m_TexChannels = 0;

	private:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;
	};
}

//Include the correct model header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_Texture.h"
#elif PLATFORM_DIRECTX
#include "Platform/DX12/Renderer/DX12_Texture.h"
#endif
