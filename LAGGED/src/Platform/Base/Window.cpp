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

	void Window::SetWindowTitle(const char* windowTitle)
	{
		m_WindowTitle = windowTitle;
		glfwSetWindowTitle(m_Window, m_WindowTitle);
	}
}