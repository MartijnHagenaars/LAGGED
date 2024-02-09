#include "Texture.h"
#include "FileIO/FileIO.h"
#include "Utility/Logger.h"

namespace LAG
{
	TextureBase::TextureBase(const HashedString& path) : Resource(path)
	{

	}
	bool TextureBase::SetPath(const std::string& path)
	{
		if (FileIO::IsPathValid(path))
		{
			SetPath(path);
			return true;
		}
		else
		{
			Logger::Error("Invalid path ({0}) used for texture.");
			return false;
		}
	}

	bool TextureBase::SetBuffer(const float* buffer, TextureFormat format)
	{
		//TODO: Implement
		return false;
	}
}
