#include "Precomp.h"
#include "GL_Texture.h"
#include "GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace LAG
{
	Texture::Texture(const Utility::String& path) :
		Resource(path)
	{
		//Load();
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

		glGenTextures(1, &m_ID);
		Bind();

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_TexWidth, m_TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(texData);

		//Apply some texture paramters before finishing loading
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);				//Use nearest texture filtering when texture is minified. 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);				//Use linear texture filtering when texture is magnified.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		return true;
	}

	bool Texture::Unload()
	{
		glDeleteTextures(1, &m_ID);
		return true;
	}

	void Texture::Bind()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void Texture::Unbind()
	{
		glDisable(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}