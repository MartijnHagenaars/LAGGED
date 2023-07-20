#pragma once
#include "Platform/Base/PlatformIncludes.h"
#include "Platform/Base/WindowBase.h"

namespace LAG::Window
{
	struct WIN32Data
	{
		//void* m_WindowData = nullptr;
		WindowEventCallbackFunc winEventCallback = NULL;

		//Store the size of the non-client window size
		unsigned int windowWidth = 0;
		unsigned int windowHeight = 0;

		const char* windowName = "";

		bool isFullscreen = false;
		bool useVSync = false;

		HWND hWnd = NULL;
	};
}


