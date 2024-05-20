#pragma once
#include "Core/Resources/Resource.h"
#include "Core/Defines.h"

namespace LAG
{
	enum class TextureFormat
	{
		FORMAT_RGBA = 0,
		FORMAT_RGB,
		FORMAT_RG,
		FORMAT_R
	};

	class TextureBase : public Resource
	{
	public:
		TextureBase();
		explicit TextureBase(const HashedString& path);
		virtual ~TextureBase() {};

		bool SetPath(const std::string& path);
		bool SetBuffer(const float* buffer, size_t bufferSize, int width, int height, TextureFormat format);

		virtual void Bind(size_t textureUnit) = 0;
		virtual void Unbind(size_t textureUnit) = 0;

		virtual bool Load() = 0;
		virtual bool Unload() = 0;

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		virtual void* GetEditorHandle() = 0;

	protected:
		int m_Width = 0;
		int m_Height = 0;
		TextureFormat m_Format = TextureFormat::FORMAT_RGB;

		bool m_LoadFromFile = false;

		size_t m_BufferSize = -1;
		const float* m_TempBuffer = nullptr;
	};
}

//Include the correct model header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_Texture.h"
#elif PLATFORM_DIRECTX

#endif
