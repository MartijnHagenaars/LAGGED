#include "FileIO.h"
#include <filesystem>

namespace LAG
{
	bool FileIO::IsPathValid(const std::string& path)
	{
		return std::filesystem::exists(path);
	}
}