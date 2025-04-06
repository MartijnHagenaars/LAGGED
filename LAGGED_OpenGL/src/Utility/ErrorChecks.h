#pragma once
#include "Utility/Logger.h"
#include "ErrorCodeLookup.h"

#ifdef DEBUG
#define LAG_GRAPHICS_CHECK(x) x; { GLenum glError; while((glError = glGetError()) != GL_NO_ERROR) {CRITICAL("OpenGL Error: [{0}][{1}]: OpenGL error: {2}", __FILE__, __LINE__, LAG::ConvertErrorToString(glError)); __debugbreak();} }
#else
#define LAG_GRAPHICS_CHECK(x) x;
#endif