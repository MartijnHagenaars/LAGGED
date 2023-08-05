#include "Precomp.h"
#include "GL_Window.h"
#include "Events/EventBase.h"
#include "Platform/Base/Window/WindowManager.h"

#include "GL_InputEnumConversion.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

namespace LAG
{
	Window::Window()
		: WindowBase()
	{
	}

	Window::~Window()
	{
		glfwTerminate();
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
	
		//Not sure if necessary
		glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_TRUE);

		glfwMakeContextCurrent(m_Window);
		SetupCallbackFunctions();
		
		if (glewInit() != GLEW_OK)
		{
			Utility::Logger::Error("Failed to initialize GLEW.");
			return;
		}
		glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);

		pressedButtonIDs;
		pressedButtonIDs.reserve(8);
		m_Initialized = true;
	}

	void Window::SetupCallbackFunctions()
	{
		//Look into simplifying this shitshow using this: GetFocus() == glfwGetWin32Window(m_Window);
		// 
		//Setup window focus callback
		auto windowFocusCallback = [](GLFWwindow* window, int focused)
		{
			if (LAG::WindowManager::Get().GetFocussedWindow() == nullptr || window != LAG::WindowManager::Get().GetFocussedWindow()->m_Window)
			{
				if (LAG::WindowManager::Get().m_AdditionalWindows.size() > 0)
				{
					if (LAG::WindowManager::Get().GetPrimaryWindow()->m_Window == window)
						LAG::WindowManager::Get().SetFocussedWindow(LAG::WindowManager::Get().GetPrimaryWindow());
					else
						for (const auto& it : LAG::WindowManager::Get().m_AdditionalWindows)
						{
							if (it->m_Window == window)
							{
								LAG::WindowManager::Get().SetFocussedWindow(it);
								return;
							}
						}
				}
				else LAG::WindowManager::Get().SetFocussedWindow(LAG::WindowManager::Get().GetPrimaryWindow());
			}
		};
		glfwSetWindowFocusCallback(m_Window, windowFocusCallback);
	}

	void Window::Update()
	{
		//Handle the releasing of button presses
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

	void Window::PresentFrame()
	{
		glfwMakeContextCurrent(m_Window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(m_Window);
	}

	bool Window::HandleWindowMessages()
	{
		if (glfwWindowShouldClose(m_Window))
		{
			return false;
		}

		return true;
	}

	void Window::SetWindowEventCallback(const WindowEventCallbackFunc& callbackFunc)
	{
		m_WinEventCallback = callbackFunc;
	}


	bool Window::CheckButtonPress(const Input::InputActionData& inputType, bool onlyDetectSinglePress)
	{
		glfwMakeContextCurrent(m_Window);

		//Detect if the button is being processed. Store the result in the buttonState varaible.
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
		
		//Process the input if it's being pressed
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
		glfwMakeContextCurrent(m_Window);

		double xPosD = 0.f, yPosD = 0.f;
		
		//Since glfwGetCursorPos only works with doubles, we need to cast it back to floats.
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