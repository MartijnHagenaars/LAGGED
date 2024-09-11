#include "GL_Cubemap.h"

#include <array>

#include <GL/glew.h>
#include <stb/stb_image.h>

#include "GL_Utility.h"
#include "GL_ErrorChecks.h"
#include "Utility/Logger.h"

namespace LAG
{
	struct CubemapFaces
	{
		std::string fileName;
		int targetID;
	};

	static std::array<CubemapFaces, 6> cubemapFaceData =
	{ {
		{"px.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X},
		{"nx.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
		{"py.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
		{"ny.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y},
		{"pz.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z},
		{"nz.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z}
	} };

	Cubemap::Cubemap(const FileIO::Directory& dir, const std::string& path)
	{
		m_Directory = FileIO::GetPath(dir, path);
	}

	bool Cubemap::Load()
	{
		if (!FileIO::IsValid(m_Directory))
		{
			CRITICAL("Cannot load cubemap: directory {} is invalid.", m_Directory);
			return false;
		}

		//Create cubemap
		LAG_GRAPHICS_CHECK(glGenTextures(1, &m_ID));
		LAG_GRAPHICS_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID));

		//Load all the data for the six faces
		const auto& files = FileIO::GetAllFilesInDirectory(m_Directory);
		for (const auto& face : cubemapFaceData)
		{
			auto it = std::find_if(files.begin(), files.end(), [&face](std::string p) { return p.find(face.fileName) != std::string::npos; });
			if (it == files.end())
				CRITICAL("No cubemap texture found for {} in directory {}.", face.fileName, m_Directory);

			std::string facePath = *it;

			int width, height, channel;
			unsigned char* faceData = stbi_load(facePath.c_str(), &width, &height, &channel, 0);
			if (faceData == nullptr)
			{
				CRITICAL("Failed to load cubemap data for face {} in directory {}.", face.fileName, m_Directory);
				stbi_image_free(faceData);
			}

			GLenum formatEnum = Utility::ConvertFormatToGLEnum(TextureFormat(channel));
			LAG_GRAPHICS_CHECK(glTexImage2D(face.targetID, 0, formatEnum, width, height, 0, formatEnum, GL_UNSIGNED_BYTE, faceData));

			stbi_image_free(faceData);
		}

		return true;
	}

	void Cubemap::Unload()
	{
		LAG_GRAPHICS_CHECK(glDeleteTextures(1, &m_ID));
	}

	void Cubemap::Bind()
	{
	}
}
