#pragma once
#include <string>

namespace LAG
{
	class FileIO
	{
	public:
		FileIO() = default;
		~FileIO() = default;

		static bool IsPathValid(const std::string& path);

		struct ImageData
		{
			unsigned char* data = nullptr;
			int width = 0;
			int height = 0;
			int channels = 0;
		};
		static bool LoadImageFromFile(const std::string& path, ImageData& imageData);
		static bool FreeImageData(ImageData& data);
	};
}

