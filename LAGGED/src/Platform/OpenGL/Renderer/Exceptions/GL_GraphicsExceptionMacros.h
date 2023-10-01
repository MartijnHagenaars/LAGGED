#pragma once
#include "GL_GraphicsException.h"

#ifdef DEBUG
#define LAG_GRAPHICS_EXCEPTION(x) x; { GLenum glError; while((glError = glGetError()) != GL_NO_ERROR) throw GraphicsException(__LINE__, __FILE__, glError ); }

#else
#define LAG_GRAPHICS_EXCEPTION(x) x;
#endif