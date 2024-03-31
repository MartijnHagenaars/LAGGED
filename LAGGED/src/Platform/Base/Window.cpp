#include "Platform/Window.h"
#include "Utility/Logger.h"

#include "GLFW/glfw3.h"

namespace LAG
{
	Window::Window(uint32_t winWidth, uint32_t winHeight, const std::string& winTitle, bool fullscreen, bool useVSync, bool centerWindow) : 
		m_WindowWidth(winWidth), m_WindowHeight(winHeight), m_WindowTitle(winTitle.c_str()),
		m_IsFullscreen(fullscreen), m_UseVSync(useVSync), m_IsOpen(false)
	{
		if (!Initialize())
			Logger::Critical("Failed to initialize window.");
		else Logger::Info("Successfully initialized window.");
	}

	bool Window::Initialize()
	{
		//If the window is already open and/or initialized
		if (m_IsOpen || glfwInit() != GLFW_TRUE)
		{
			Logger::Error("Window already initialized.");
			return false;
		}

		//Create the window. 
		m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, m_WindowTitle, NULL, NULL);
		if (m_Window == nullptr)
		{
			Logger::Critical("Failed to create GLFW window");
			Shutdown();
			return false;
		}

		glfwSetWindowUserPointer(m_Window, this);
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);

		PlatformInitialize();

		//Not sure if necessary
		glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_TRUE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
		pressedButtonIDs.reserve(8);

		ImGuiInitialize();



		//Window is ready to be used! Mark as open!
		m_IsOpen = true;
		return true;
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
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
	
	void Window::SetWindowTitle(const char* windowTitle)
	{
		m_WindowTitle = windowTitle;
		glfwSetWindowTitle(m_Window, m_WindowTitle);
	}
}