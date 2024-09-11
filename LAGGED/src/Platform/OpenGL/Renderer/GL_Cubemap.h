#pragma once
#include <string>

#include "Core/FileIO/FileIO.h"
#include "Platform/Base/Renderer/Buffer.h"

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
		ArrayBuffer m_Buffer;

		unsigned int m_ID = 0;
		std::string m_Directory;
	};
}
