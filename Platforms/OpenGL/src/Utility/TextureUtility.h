#pragma once
#include <GL/glew.h>
#include "Platform/Defines.h"

namespace LAG
{
	static GLenum ConvertFormatToGLEnum(TextureFormat format)
	{
		switch (format)
		{
		default: case TextureFormat::FORMAT_RGB: return GL_RGB;
		case TextureFormat::FORMAT_RGBA: return GL_RGBA;
		case TextureFormat::FORMAT_RG: return GL_RG;
		case TextureFormat::FORMAT_R: return GL_RED;
		}
	}
}