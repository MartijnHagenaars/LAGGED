#include "Texture.h"
#include "FileIO/FileIO.h"
#include "Utility/Logger.h"

namespace LAG
{
	TextureBase::TextureBase() : Resource(HashedString()), m_LoadFromFile(false) {}
	TextureBase::TextureBase(const HashedString& path) : Resource(path), m_LoadFromFile(true) {}

	bool TextureBase::SetBuffer(const unsigned char* buffer, size_t bufferSize, int width, int height, TextureFormat format)
	{
		if (width <= 0 && height <= 0)
		{
			ERROR("Invalid texture size: {0}x{1}.", width, height);
			return false;
		}

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

			m_Format = format;
			m_TempBuffer = buffer;
			m_BufferSize = bufferSize;

			m_Width = width;
			m_Height = height;

			return true;
		}
		else
		{
			ERROR("Buffer is invalid.");
			return false;
		}
	}
}
