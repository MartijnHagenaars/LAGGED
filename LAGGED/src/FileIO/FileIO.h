#pragma once
#include <string>
#include <vector>

namespace LAG
{
	class FileIO
	{
	public:
		FileIO() = default;
		~FileIO() = delete;

		enum class Directory
		{
			Root,
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

		static std::vector<std::string> GetAllFilesInDirectory(Directory dir, const std::string& path);

		static std::vector<std::string> GetAllSubDirectories(Directory dir, const std::string& path);
	};
}

