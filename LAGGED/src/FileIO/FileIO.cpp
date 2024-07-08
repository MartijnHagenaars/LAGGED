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
		case Directory::Models: return "Resources/Models/";
		case Directory::Shaders: return "Resources/Shaders/";
		case Directory::Saves: return "Resources/Saves";
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
}