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

		void Bind();
		void Unbind();

	private:
		unsigned int m_ID = 0;
		std::string m_Directory;
	};
}
