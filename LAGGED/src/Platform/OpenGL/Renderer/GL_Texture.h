#pragma once
#include "Core/Resources/Texture.h"

namespace LAG
{
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
		~Texture() override;

		bool Load() override;
		bool Unload() override;

		void Bind(size_t textureUnit) override;
		void Unbind(size_t textureUnit)  override;

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
