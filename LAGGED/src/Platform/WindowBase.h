#pragma once
#include "Core/Defines.h"
#include "Events/WindowEvents.h"
#include <functional>

namespace LAG::Window
{
	using WindowEventCallbackFunc = std::function<void(EventBase&)>;

	void Initialize(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync = true, bool centerWindow = true);
	void Shutdown(); 

	// Handles the message pump
	// &exitCodeOut: when program wants to exit, the exit code is assigned to the referenced argument. 
	// Return value: returns FALSE when the program wants to exit
	bool HandleWindowMessages(int& exitCodeOut);

	void SetWindowEventCallback(const WindowEventCallbackFunc& callbackFunc);

	LAG_API void SetWindowName(const char* windowName);

	LAG_API unsigned int GetWidth();
	LAG_API unsigned int GetHeight();

	LAG_API unsigned int GetClientWidth();
	LAG_API unsigned int GetClientHeight();

	const void* GetWindowData();
}