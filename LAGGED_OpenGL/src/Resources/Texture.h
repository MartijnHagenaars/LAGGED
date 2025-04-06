#pragma once
#include "Platform/Resources/Texture.h"

namespace LAG
{
	enum class TextureFormat
	{
		FORMAT_RGBA = 0,
		FORMAT_RGB,
		FORMAT_RG,
		FORMAT_R
	};

	typedef unsigned int GLenum;
	class Texture : public TextureBase
	{
		friend class Model;
		friend class ResourceManager;
	public:
		Texture() = default;

		/// <summary>
		/// Load a texture from file
		/// </summary>
		/// <param name="path">The path to where the texture file is stored.</param>
		explicit Texture(const HashedString& path);
		explicit Texture(const HashedString& handle, const unsigned char* buffer, size_t bufferSize, int width, int height, TextureFormat format);
		~Texture() override;

		bool Load() override;
		bool Unload() override;

		bool SetBuffer(const unsigned char* buffer, size_t bufferSize, int width, int height, TextureFormat format);

		void Bind(size_t textureUnit) override;
		void Unbind(size_t textureUnit)  override;

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		void* GetEditorHandle() override { return (void*)(intptr_t)m_ID; }

	private:
		struct TextureData
		{
			unsigned char* data = nullptr;
			int width = 0;
			int height = 0;
			int channels = 0;
		};
		bool LoadTextureData(TextureData& textureData, const std::string& path);
		void FreeTextureData(TextureData& textureData);

		GLenum ConvertFormatToGLEnum(TextureFormat format);
		unsigned int m_ID = 0;
	};

}
