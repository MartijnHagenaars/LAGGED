#include "Precomp.h"
#include "GL_Texture.h"
#include "GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Exceptions/GL_GraphicsExceptionMacros.h"

namespace LAG
{
	Texture::Texture(const Utility::String& path) :
		TextureBase(path)
	{
	}

	Texture::~Texture()
	{

	}

	bool Texture::Load()
	{
		std::string filePath = GetPath().GetString();
		//Check if the texture hasn't been loaded yet. 
		if (m_ID != 0)
		{
			Utility::Logger::Warning("Texture \"{0}\" has already been loaded.", filePath);
			return false;
		}

		LAG_GRAPHICS_EXCEPTION(glGenTextures(1, &m_ID));
		LAG_GRAPHICS_EXCEPTION(Bind(0));

		//Load image data
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* texData = stbi_load(filePath.c_str(), &m_TexWidth, &m_TexHeight, &m_TexChannels, 0);
		if (texData == nullptr)
		{
			Utility::Logger::Error("Failed to load texture data for texture with the following path: {0}", filePath);
			glDeleteTextures(1, &m_ID);
			return false;
		}

		//Apply image data
		LAG_GRAPHICS_EXCEPTION(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_TexWidth, m_TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData));
		LAG_GRAPHICS_EXCEPTION(glGenerateMipmap(GL_TEXTURE_2D));
		stbi_image_free(texData);

		//Apply some texture paramters before finishing loading
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));				//Use nearest texture filtering when texture is minified. 
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));				//Use linear texture filtering when texture is magnified.
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

		return true;
	}

	bool Texture::Unload()
	{
		glDeleteTextures(1, &m_ID);
		return true;
	}

	int GetTextureUnit(size_t textureUnit)
	{
		switch (textureUnit)
		{
		case 0: return GL_TEXTURE0;
		case 1: return GL_TEXTURE1;
		case 2: return GL_TEXTURE2;
		case 3: return GL_TEXTURE3;
		case 4: return GL_TEXTURE4;
		case 5: return GL_TEXTURE5;
		case 6: return GL_TEXTURE6;
		case 7: return GL_TEXTURE7;
		default: return GL_TEXTURE0;
		}
	}

	void Texture::Bind(size_t textureUnit)
	{
		LAG_GRAPHICS_EXCEPTION(glActiveTexture(GetTextureUnit(textureUnit));)
		LAG_GRAPHICS_EXCEPTION(glBindTexture(GL_TEXTURE_2D, m_ID));
	}

	void Texture::Unbind(size_t textureUnit)
	{
		LAG_GRAPHICS_EXCEPTION(glBindTexture(GL_TEXTURE_2D, 0));
	}

}