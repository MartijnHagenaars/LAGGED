#pragma once
#include "Core/Resources/Texture.h"

namespace LAG
{
	class Texture : public TextureBase
	{
		friend class Model;
		friend class ResourceManager;
	public:
		Texture() = delete;

		/// <summary>
		/// Load a texture from file
		/// </summary>
		/// <param name="path">The path to where the texture file is stored.</param>
		explicit Texture(const HashedString& path);

		//Texture(const HashedString& path);

		~Texture() override;

		void Bind(size_t textureUnit) override;
		void Unbind(size_t textureUnit)  override;

		void* GetEditorHandle() override { return (void*)(intptr_t)m_ID; }

	private:
		bool Load() override;
		bool Unload() override;

		unsigned int m_ID = 0;
	};

}
