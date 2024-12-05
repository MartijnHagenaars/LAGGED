#pragma once
#include "Platform/Base/RendererTypes.h"

namespace LAG
{
	namespace Utility
	{
		typedef unsigned int GLenum;
		GLenum ConvertFormatToGLEnum(TextureFormat format);
	}
}
