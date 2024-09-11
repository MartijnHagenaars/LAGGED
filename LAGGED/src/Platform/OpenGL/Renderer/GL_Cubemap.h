#pragma once
#include <string>

#include "Core/FileIO/FileIO.h"

namespace LAG
{
	class Cubemap
	{
	public:
		Cubemap(const FileIO::Directory& dir, const std::string& path);
		~Cubemap() = default;

		bool Load();
		void Unload();

	private:
		std::string m_Directory;
	};
}
