#pragma once
#include "Core/Defines.h"
#include <functional>

#include "Core/Input/Input.h"

namespace LAG
{
	class EventBase;
	using WindowEventCallbackFunc = std::function<void(EventBase&)>;

	class WindowBase
	{
	public:
		WindowBase() {};
		virtual ~WindowBase() {};

		// Handles the message pump
		// &exitCodeOut: when program wants to exit, the exit code is assigned to the referenced argument. 
		// Return value: returns FALSE when the program wants to exit
		virtual bool HandleWindowMessages() = 0;

		virtual void PresentFrame() = 0;

		virtual bool CheckButtonPress(const Input::InputActionData& inputType, bool onlyDetectSinglePress) = 0;
		virtual void GetMousePosition(float& xPos, float& yPos) = 0;

		virtual void SetWindowEventCallback(const WindowEventCallbackFunc& callbackFunc) = 0;

		virtual void SetWindowName(const char* windowName) = 0;

		virtual const char* GetWindowName() const { return m_WindowName; }
		virtual unsigned int GetWidth() const { return m_WindowWidth; }
		virtual unsigned int GetHeight() const { return m_WindowHeight; }

		virtual unsigned int GetHalfWidth() const { return m_WindowHalfWidth; }
		virtual unsigned int GetHalfHeight() const { return m_WindowHalfHeight; }

		virtual unsigned int GetNonClientWidth() = 0;
		virtual unsigned int GetNonClientHeight() = 0;

	protected:
		virtual void Initialize(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync = true, bool centerWindow = true) = 0;
		virtual void Update() = 0;

		//Window property values
		unsigned int m_WindowWidth = 0;
		unsigned int m_WindowHeight = 0;
		unsigned int m_WindowHalfWidth = 0;
		unsigned int m_WindowHalfHeight = 0;

		const char* m_WindowName = "";
		bool m_IsFullscreen = false;
		bool m_UseVSync = false;

		bool m_Initialized = false;
	};
}