#include "GL_Utility.h"
#include <GL/glew.h>

GLenum LAG::Utility::ConvertFormatToGLEnum(TextureFormat format)
{
	switch (format)
	{
	default: case TextureFormat::FORMAT_RGB: return GL_RGB;
	case TextureFormat::FORMAT_RGBA: return GL_RGBA;
	case TextureFormat::FORMAT_RG: return GL_RG;
	case TextureFormat::FORMAT_R: return GL_RED;
	}
}
