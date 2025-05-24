#include "Platform/Resources/Cubemap.h"

#include <array>

#include <GL/glew.h>
#include <stb/stb_image.h>

#include "Core/IO/FileIO.h"
#include "Utility/GL_ErrorChecks.h"
#include "Utility/Logger.h"
#include "Utility/TextureUtility.h"

namespace LAG
{
	struct GlCubemapData
	{
		unsigned int id = 0;
	};

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

	Cubemap::Cubemap(const HashedString& path) :
		Resource(path)
	{
	}

	bool Cubemap::Load()
	{
		const std::string& pathStr = GetPath().GetString();
		if (!FileIO::IsValid(GetPath().GetString()))
		{
			CRITICAL("Cannot load cubemap: directory {} is invalid.", pathStr);
			return false;
		}

		m_DataPtr = new GlCubemapData;
		GlCubemapData* data = static_cast<GlCubemapData*>(m_DataPtr);

		//Create cubemap
		LAG_GRAPHICS_CHECK(glGenTextures(1, &data->id));
		LAG_GRAPHICS_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, data->id));

		//Load all the data for the six faces
		const auto& files = FileIO::GetAllFilesInDirectory(pathStr);
		for (const auto& face : cubemapFaceData)
		{
			auto it = std::find_if(files.begin(), files.end(), [&face](std::string p) { return p.find(face.fileName) != std::string::npos; });
			if (it == files.end())
				CRITICAL("No cubemap texture found for {}.", pathStr);

			int width, height, channel;
			unsigned char* faceData = stbi_load(it->c_str(), &width, &height, &channel, 0);
			if (faceData == nullptr)
			{
				CRITICAL("Failed to load cubemap data for face {}.", pathStr);
				stbi_image_free(faceData);
			}

			GLenum formatEnum = ConvertFormatToGLEnum(TextureFormat(channel));
			LAG_GRAPHICS_CHECK(glTexImage2D(face.targetID, 0, formatEnum, width, height, 0, formatEnum, GL_UNSIGNED_BYTE, faceData));

			stbi_image_free(faceData);
		}

		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

		return true;
	}

	bool Cubemap::Unload()
	{
		LAG_GRAPHICS_CHECK(glDeleteTextures(1, &static_cast<GlCubemapData*>(m_DataPtr)->id));
		if (m_DataPtr)
			delete m_DataPtr;
		m_DataPtr = nullptr;

		return true;
	}

	void Cubemap::Bind()
	{
		LAG_GRAPHICS_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<GlCubemapData*>(m_DataPtr)->id));
	}

	void Cubemap::Unbind()
	{
		LAG_GRAPHICS_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
}
