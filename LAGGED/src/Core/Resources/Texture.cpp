#include "Texture.h"
#include "FileIO/FileIO.h"
#include "Utility/Logger.h"

namespace LAG
{
	TextureBase::TextureBase(const HashedString& path) : Resource(path) {}

	bool TextureBase::SetPath(const std::string& path)
	{
		if (FileIO::IsPathValid(path))
		{
			m_LoadFromFile = true;
			delete m_TempBuffer;
			m_TempBuffer = nullptr;

			SetPath(path);
			return true;
		}
		else
		{
			Logger::Error("Invalid path ({0}) used for texture.");
			return false;
		}
	}

	bool TextureBase::SetBuffer(const float* buffer, size_t bufferSize, TextureFormat format)
	{
		if (buffer)
		{
			if (format == TextureFormat::FORMAT_RGBA && (bufferSize % 4) != 0)
				return false;
			else if (format == TextureFormat::FORMAT_RGB && (bufferSize % 3) != 0)
				return false;
			else if (format == TextureFormat::FORMAT_RG && (bufferSize % 2) != 0)
				return false;
			else if (format == TextureFormat::FORMAT_R && (bufferSize % 1) != 0)
				return false;

			m_LoadFromFile = false;
			SetPath("");

			m_Format = format;
			m_TempBuffer = buffer;
			m_BufferSize = bufferSize;

			return true;
		}
		else
		{
			Logger::Error("Buffer is invalid.");
			return false;
		}
	}
}
