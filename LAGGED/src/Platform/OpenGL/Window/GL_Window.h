#pragma once
#include "Platform/Base/WindowBase.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace LAG::Window
{
	struct WindowData
	{
		GLFWwindow* window = nullptr;
		WindowEventCallbackFunc winEventCallback = NULL;
	};
}