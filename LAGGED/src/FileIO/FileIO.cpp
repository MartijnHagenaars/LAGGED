#include "FileIO.h"
#include <filesystem>
#include "Utility/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace LAG
{
	bool FileIO::IsPathValid(const std::string& path)
	{
		return std::filesystem::exists(path);
	}

	bool FileIO::LoadImageFromFile(const std::string& path, ImageData& data)
	{
		if (!IsPathValid(path))
		{
			Logger::Error("Invalid path: {0}", path);
			return false;
		}

		//Load image data
		stbi_set_flip_vertically_on_load(true);
		data.data = stbi_load(path.c_str(), &data.width, &data.height, &data.channels, 0);
		if (data.data == nullptr)
		{
			Logger::Error("Failed to load image from location \"{0}\": data is NULL", path);
			return false;
		}

		return true;
	}

	bool FileIO::FreeImageData(ImageData& data)
	{
		if (!data.data)
		{
			stbi_image_free(data.data);
			data.width = 0;
			data.height = 0;
			data.channels = 0;
			return true;
		}
		else return false;
	}
}