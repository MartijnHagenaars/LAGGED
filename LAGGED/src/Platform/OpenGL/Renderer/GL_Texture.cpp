#include "GL_Texture.h"
#include "GL/glew.h"

#include "FileIO/FileIO.h"
#include "Utility/Logger.h"
#include "GL_ErrorChecks.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace LAG
{
	Texture::Texture(const HashedString& path) :
		TextureBase(path)
	{
	}

	Texture::~Texture()
	{

	}

	bool Texture::Load()
	{
		if (IsLoaded())
		{
			WARNING("Tried to load an already loaded Texture class.");
			return false;
		}

		//Load from file
		if (m_LoadFromFile && GetPath().GetString().length() > 0)
		{
			std::string filePath = GetPath().GetString();
			//Check if the texture hasn't been loaded yet. 
			if (m_ID != 0)
			{
				WARNING("Texture \"{0}\" has already been loaded.", filePath);
				return false;
			}

			TextureData texData;
			if (!LoadTextureData(texData, filePath))
			{
				ERROR("Failed to load texture.");
				return false;
			}

			//Assign correct channel enum
			if (texData.channels == 4) m_Format = TextureFormat::FORMAT_RGBA;
			else if (texData.channels == 3) m_Format = TextureFormat::FORMAT_RGB;
			else if (texData.channels == 2) m_Format = TextureFormat::FORMAT_RG;
			else if (texData.channels == 1) m_Format = TextureFormat::FORMAT_R;

			m_Width = texData.width;
			m_Height = texData.height;

			//Create GL texture
			LAG_GRAPHICS_CHECK(glGenTextures(1, &m_ID));
			LAG_GRAPHICS_CHECK(Bind(0));

			//Apply image data
			LAG_GRAPHICS_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, ConvertFormatToGLEnum(m_Format), m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData.data));
			LAG_GRAPHICS_CHECK(glGenerateMipmap(GL_TEXTURE_2D));

			FreeTextureData(texData);
		}
		else if (!m_LoadFromFile && m_TempBuffer)
		{
			//Create GL texture
			LAG_GRAPHICS_CHECK(glGenTextures(1, &m_ID));
			LAG_GRAPHICS_CHECK(Bind(0));

			LAG_GRAPHICS_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, ConvertFormatToGLEnum(m_Format), m_Width, m_Height, 0, ConvertFormatToGLEnum(m_Format), GL_FLOAT, m_TempBuffer));
			LAG_GRAPHICS_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
		}
		else 
		{
			ERROR("Trying to load invalid texture.");
			return false;
		}

		//Apply some texture paramters before finishing loading
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));				//Use nearest texture filtering when texture is minified. 
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));				//Use linear texture filtering when texture is magnified.
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
		
		//Swizzle the texture if only the red channel is used. This prevents textures from looking red and instead grayscales them
		if (m_Format == TextureFormat::FORMAT_R)
		{
			GLint swizzle[4] = { GL_RED, GL_RED, GL_RED, GL_ONE };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		}

		SetLoaded(true);
		return true;
	}

	bool Texture::Unload()
	{
		glDeleteTextures(1, &m_ID);
		SetLoaded(false);
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
		LAG_GRAPHICS_CHECK(glActiveTexture(GetTextureUnit(textureUnit));)
		LAG_GRAPHICS_CHECK(glBindTexture(GL_TEXTURE_2D, m_ID));
	}

	void Texture::Unbind(size_t textureUnit)
	{
		LAG_GRAPHICS_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	}

	bool Texture::LoadTextureData(TextureData& texData, const std::string& path)
	{
		//Load image data
		
		stbi_set_flip_vertically_on_load(true);
		texData.data = stbi_load(path.c_str(), &texData.width, &texData.height, &texData.channels, 0);
		if (texData.data == nullptr)
		{
			CRITICAL("Failed to load image from location \"{0}\": data is NULL", path);
			return false;
		}
		else return true;
	}

	void Texture::FreeTextureData(TextureData& textureData)
	{
		if (textureData.data != nullptr)
		{
			stbi_image_free(textureData.data);
			textureData.data = nullptr;

			textureData.width = 0;
			textureData.height = 0;
			textureData.channels = 0;
		}
	}

	GLenum Texture::ConvertFormatToGLEnum(TextureFormat format)
	{
		switch (format)
		{
		default: case TextureFormat::FORMAT_RGB: return GL_RGB;
		case TextureFormat::FORMAT_RGBA: return GL_RGBA;
		case TextureFormat::FORMAT_RG: return GL_RG;
		case TextureFormat::FORMAT_R: return GL_RED;
		}
	}

}