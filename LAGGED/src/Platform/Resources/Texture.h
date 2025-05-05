#pragma once
#include "Core/Resources/Resource.h"

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
	class Texture : public Resource
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

		void Bind(size_t textureUnit);
		void Unbind(size_t textureUnit);

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		void* GetEditorHandle();

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

		int m_Width = 0;
		int m_Height = 0;
		TextureFormat m_Format = TextureFormat::FORMAT_RGB;

		bool m_LoadFromFile = false;

		size_t m_BufferSize = -1;
		const unsigned char* m_TempBuffer = nullptr;

		void* m_DataPtr = nullptr;
	};

}
