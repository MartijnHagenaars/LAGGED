#include "Precomp.h"
#include "GL_Window.h"
#include "Events/EventBase.h"

#include "GL_InputEnumConversion.h"
#include <unordered_set>

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

namespace LAG
{
	std::unordered_set<size_t> pressedButtonIDs(8); //Note: This is going to be moved when I add the window class, so ignore this for now. 

	Window::Window()
		: WindowBase()
	{
	}

	void Window::Initialize(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync, bool centerWindow)
	{
		if (m_Initialized)
		{
			Utility::Logger::Error("Window already initialized.");
			return;
		}

		if (glfwInit() != GLFW_TRUE)
		{
			Utility::Logger::Critical("GLFW failed to initialize.");
			return;
		}

		//TODO: Maybe check for most recent version of OpenGL 4? 
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		m_Window = glfwCreateWindow(winWidth, winHeight, "Hello world!", NULL, NULL);
		if (m_Window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_Window);

		m_Initialized = true;
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::PresentFrame()
	{
	}

	bool Window::HandleWindowMessages(int& exitCodeOut)
	{
		glfwPollEvents();
		if (glfwWindowShouldClose(m_Window))
		{
			exitCodeOut = 0;
			return false;
		}

		return true;
	}

	/*void Window::PresentFrame()
	{
		glfwSwapBuffers(window);
	}*/

	void Window::SetWindowEventCallback(const WindowEventCallbackFunc& callbackFunc)
	{
		m_WinEventCallback = callbackFunc;
	}

	void Window::Update()
	{
		if (pressedButtonIDs.size() > 0)
		{
			for (auto it = pressedButtonIDs.begin(); it != pressedButtonIDs.end();)
			{
				if (!CheckButtonPress(*Input::GetInputAction(*it), false))
					it = pressedButtonIDs.erase(it);
				else ++it;
			}
		}

	}

	bool Window::CheckButtonPress(const Input::InputActionData& inputType, bool onlyDetectSinglePress)
	{
		int buttonState = 0;
		Input::InputDeviceType deviceType = GetInputDeviceType(inputType.type);

		if (deviceType == Input::InputDeviceType::LAG_UNKNOWN)
			return false;
		else if (deviceType == Input::InputDeviceType::LAG_KEYBOARD)
		{
			int buttonType = ConvertLAGInputToGLFWInput(inputType.type);
			buttonState = glfwGetKey(m_Window, buttonType);
		}
		else if (deviceType == Input::InputDeviceType::LAG_MOUSE)
		{
			buttonState = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
		}
		
		if (buttonState > 0)
		{
			if (onlyDetectSinglePress)
			{
				if (pressedButtonIDs.find(inputType.actionID) == pressedButtonIDs.end())
					pressedButtonIDs.emplace(inputType.actionID);
				else return false;
			}
			
			return true;
		}

		return false;
	}

	void Window::GetMousePosition(float& xPos, float& yPos)
	{
		double xPosD = 0.f, yPosD = 0.f;
		
		glfwGetCursorPos(m_Window, &xPosD, &yPosD);
		xPos = static_cast<float>(xPosD);
		yPos = static_cast<float>(yPosD);
	}

	void Window::SetWindowName(const char* windowName)
	{
		m_WindowName = windowName;
		glfwSetWindowTitle(m_Window, m_WindowName);
	}

	unsigned int Window::GetNonClientWidth()
	{
		RECT betterRect = { 0 };
		GetWindowRect(glfwGetWin32Window(m_Window), &betterRect);
		return betterRect.right;
	}

	unsigned int Window::GetNonClientHeight()
	{
		RECT nonClientRect = { 0 };
		GetWindowRect(glfwGetWin32Window(m_Window), &nonClientRect);
		return nonClientRect.bottom;
	}
}