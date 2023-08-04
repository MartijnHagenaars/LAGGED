#pragma once
#include "Platform/Base/PlatformIncludes.h"
#include "Platform/Base/WindowBase.h"

namespace LAG::Window
{
	struct WIN32Data
	{
		//void* m_WindowData = nullptr;
		WindowEventCallbackFunc winEventCallback = NULL;

		HWND hWnd = NULL;
	};
}


