#include "FileIO.h"
#include <filesystem>
#include "Utility/Logger.h"

namespace LAG
{

	bool FileIO::IsValid(Directory directory, const std::string& path)
	{
		return std::filesystem::exists(GetPath(directory, path));
	}

	std::string FileIO::GetPath(Directory dir)
	{
		switch (dir)
		{
		case Directory::Root: return "/";
		case Directory::Models: return "res/Assets/Models/";
		case Directory::Shaders: return "res/Shaders/OpenGL/";
		case Directory::Saves: return "res/Saves";
		case Directory::Logs: return "Logs/";
		default:
			Logger::Critical("Incorrect directory type.");
			return std::string();
		}
	}

	std::string FileIO::GetPath(Directory dir, const std::string& path)
	{
		return std::string(GetPath(dir) + path);
	}

	std::vector<std::string> FileIO::GetAllFilesInDirectory(Directory dir, const std::string& path)
	{
		if (!IsValid(dir, path))
		{
			Logger::Critical("Cannot get all files in directory: path ({0}) is incorrect.", GetPath(dir, path));
			return std::vector<std::string>();
		}

		std::vector<std::string> files;
		for (const auto& it : std::filesystem::directory_iterator(GetPath(dir, path)))
			if (!it.is_directory())
				files.emplace_back(it.path().string());

		return files;
	}

	std::vector<std::string> FileIO::GetAllSubDirectories(Directory dir, const std::string& path)
	{
		if (!IsValid(dir, path))
		{
			Logger::Error("Cannot get all subdirectories in directory: path ({0}) is incorrect.", path);
			return std::vector<std::string>();
		}
		std::vector<std::string> directories;
		for (const auto& it : std::filesystem::directory_iterator(GetPath(dir, path)))
			if (it.is_directory())
				directories.emplace_back(it.path().string());

		return directories;
	}
}