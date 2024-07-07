#pragma once
#include <string>

namespace LAG
{
	class FileIO
	{
	public:
		FileIO() = default;
		~FileIO() = delete;

		enum class Directory
		{
			Models,
			Shaders,
			Saves,
			Logs
		};

		/// <summary>
		/// Check if a file path is valid.
		/// </summary>
		/// <param name="directory">Directory to look in</param>
		/// <param name="path">Relative path to the file</param>
		/// <returns>Returns true when file path is valid.</returns>
		static bool IsValid(Directory directory, const std::string& path);

		/// <summary>
		/// Returns the path of the directory enum
		/// </summary>
		static std::string GetPath(Directory dir);

		/// <summary>
		/// Combines the paths of a certain directory enum with a relative path.
		/// </summary>
		/// <param name="dir">Directory enum</param>
		/// <param name="path">Relative path</param>
		/// <returns>A string containing both the directory path and the relative path.</returns>
		static std::string GetPath(Directory dir, const std::string& path);

		struct ImageData
		{
			unsigned char* data = nullptr;
			int width = 0;
			int height = 0;
			int channels = 0;
		};
		static bool LoadImageFromFile(Directory dir, const std::string& path, ImageData& imageData);
		static bool FreeImageData(ImageData& data);
	};
}

