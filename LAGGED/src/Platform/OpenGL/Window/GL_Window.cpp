#include "Precomp.h"
#include "GL_Window.h"

namespace LAG::Window
{
	WindowData* winData = nullptr;

	bool Initialize(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync, bool centerWindow)
	{
		if (winData == nullptr)
			winData = new WindowData;
		else
		{
			Utility::Logger::Error("GL Window already initialized, or WindowData is already constructed.");
			return false;
		}

		if (glfwInit() != GLFW_TRUE)
		{
			Utility::Logger::Critical("GLFW failed to initialize.");
			return false;
		}

		//TODO: Maybe check for most recent version of OpenGL 4? 
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		winData->window = glfwCreateWindow(winWidth, winHeight, "Hello world!", NULL, NULL);
		if (winData->window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(winData->window);


		//glGetString(GL_VERSION);

		return true;
	}
	
	void Shutdown()
	{
		glfwTerminate();
	}

	bool HandleWindowMessages(int& exitCodeOut)
	{
		while (!glfwWindowShouldClose(winData->window))
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(winData->window);
			glfwPollEvents();
		}

		exitCodeOut = 0;
		return false;
	}


	void SetWindowEventCallback(const WindowEventCallbackFunc& callbackFunc)
	{

	}

	const void* GetWindowData()
	{
		return winData;
	}
}