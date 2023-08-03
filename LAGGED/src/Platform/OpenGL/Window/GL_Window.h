#pragma once
#include "Platform/Base/Window/WindowBase.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace LAG
{
	struct WindowData
	{
		GLFWwindow* window = nullptr;
		WindowEventCallbackFunc winEventCallback = NULL;
	};
}