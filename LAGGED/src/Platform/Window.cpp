#include "Platform/Window.h"

#include "Utility/Logger.h"
#include "Utility/InputEnumConversion.h"

#include <GLFW/glfw3.h>

namespace LAG
{
	Window::Window(uint32_t winWidth, uint32_t winHeight, const std::string& winTitle, bool fullscreen, bool useVSync, bool centerWindow) : 
		m_WindowWidth(winWidth), m_WindowHeight(winHeight), m_WindowTitle(winTitle.c_str()),
		m_IsFullscreen(fullscreen), m_UseVSync(useVSync), m_IsOpen(false)
	{
		if (!Initialize())
			CRITICAL("Failed to initialize window.");
		else INFO("Successfully initialized window.");
	}

	bool Window::Initialize()
	{
		//If the window is already open and/or initialized
		if (m_IsOpen || glfwInit() != GLFW_TRUE)
		{
			ERROR("Window already initialized.");
			return false;
		}

		//Create the window. 
		m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, m_WindowTitle, NULL, NULL);
		if (m_Window == nullptr)
		{
			CRITICAL("Failed to create GLFW window");
			Shutdown();
			return false;
		}

		glfwSetWindowUserPointer(m_Window, this);
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);

		glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_TRUE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
		pressedButtonIDs.reserve(8);

		// Now run platform-specific init functions
		PlatformInitialize();
		ImGuiInitialize();

		// Window is ready to be used! Mark as open!
		m_IsOpen = true;
		return true;
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();

		m_IsOpen = false;
	}

	void Window::Update()
	{
		HandleWindowMessages();

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

	bool Window::HandleWindowMessages()
	{
		//Process all pending events
		glfwPollEvents();

		//Check if window should close
		if (glfwWindowShouldClose(m_Window))
			m_IsOpen = false;

		//Setup window resize callback
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* glfwWindow, int width, int height)
			{
				//Ensure that width/height cannot go below zero
				if (width <= 0)
					width = 1;
				if (height <= 0)
					height = 1;

				Window* window = static_cast<LAG::Window*>(glfwGetWindowUserPointer(glfwWindow));
				window->m_WindowWidth = width;
				window->m_WindowHeight = height;
				window->m_ResizeCallbackFunc(width, height); 
			});

		return true;
	}

	bool Window::CheckButtonPress(const Input::InputActionData& inputType, bool onlyDetectSinglePress)
	{
		// Detect if the button is being processed. Store the result in the buttonState varaible.
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

		// Process the input if it's being pressed
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

		//Since glfwGetCursorPos only works with doubles, we need to cast it back to floats.
		glfwGetCursorPos(m_Window, &xPosD, &yPosD);
		xPos = static_cast<float>(xPosD);
		yPos = static_cast<float>(yPosD);
	}
	
	void Window::SetWindowTitle(const char* windowTitle)
	{
		m_WindowTitle = windowTitle;
		glfwSetWindowTitle(m_Window, m_WindowTitle);
	}
}