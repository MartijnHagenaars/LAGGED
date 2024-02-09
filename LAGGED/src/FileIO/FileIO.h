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
	};
}

