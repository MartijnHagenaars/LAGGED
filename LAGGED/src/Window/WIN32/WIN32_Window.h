#pragma once
#include "Platform/PlatformIncludes.h"
#include "Platform/WindowBase.h"

namespace LAG::Window
{
	struct WIN32Data
	{
		//void* m_WindowData = nullptr;
		WindowEventCallbackFunc winEventCallback = NULL;

		unsigned int windowWidth = 0;
		unsigned int windowHeight = 0;
		const char* windowName = "";

		bool isFullscreen = false;
		bool useVSync = false;

		HWND hWnd = NULL;
	};
}


