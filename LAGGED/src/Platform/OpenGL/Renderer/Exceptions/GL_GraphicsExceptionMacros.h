#pragma once
#include "Utility/Logger.h"
#include "Platform/OpenGL/Renderer/GL_ErrorCodeLookup.h"

//Instead of this, look into using OpenGL Error callbacks. Maybe use glDebugMessageCallback
//https://www.khronos.org/opengl/wiki/OpenGL_Error#Catching_errors_(the_easy_way)

#ifdef DEBUG
#define LAG_GRAPHICS_EXCEPTION(x) x; { GLenum glError; while((glError = glGetError()) != GL_NO_ERROR) {LAG::Logger::Critical("OpenGL Error: [{0}][{1}]: OpenGL error: {2}", __FILE__, __LINE__, LAG::ConvertErrorToString(glError)); __debugbreak();} }
#else
#define LAG_GRAPHICS_EXCEPTION(x) x;
#endif