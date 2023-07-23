#pragma once
#include "Core/Defines.h"
#include "Events/WindowEvents.h"
#include <functional>

#include "Core/Input/Input.h"

namespace LAG::Window
{
	using WindowEventCallbackFunc = std::function<void(EventBase&)>;

	bool Initialize(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync = true, bool centerWindow = true);
	void Shutdown(); 

	// Handles the message pump
	// &exitCodeOut: when program wants to exit, the exit code is assigned to the referenced argument. 
	// Return value: returns FALSE when the program wants to exit
	bool HandleWindowMessages(int& exitCodeOut);

	void SetWindowEventCallback(const WindowEventCallbackFunc& callbackFunc);

	bool CheckButtonPress(Input::InputActionData& inputType);


	LAG_API void SetWindowName(const char* windowName);

	LAG_API unsigned int GetWidth();
	LAG_API unsigned int GetHeight();

	LAG_API unsigned int GetNonClientWidth();
	LAG_API unsigned int GetNonClientHeight();

	const void* GetWindowData();
}