#include "Precomp.h"
#include "GL_Texture.h"
#include "GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


LAG::Texture::Texture(std::string path)
{
	if (!LoadTexture(path))
		Utility::Logger::Error("Failed to load texture.");
}

LAG::Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

bool LAG::Texture::LoadTexture(const std::string& path)
{
	glGenTextures(1, &m_ID);
	Bind();

	stbi_uc* texData = stbi_load(path.c_str(), &m_TexWidth, &m_TexHeight, &m_TexChannels, 0);
	if (texData == nullptr)
	{
		Utility::Logger::Error("Failed to load texture data for texture with the following path: {0}", path);
		glDeleteTextures(1, &m_ID);
		return false;
	}

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

void LAG::Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void LAG::Texture::Unbind()
{
	glDisable(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
