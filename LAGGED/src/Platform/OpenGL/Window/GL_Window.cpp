#include "Precomp.h"
#include "GL_Window.h"

#include "GL_InputEnumConversion.h"
#include <unordered_set>

namespace LAG
{
	std::unordered_set<size_t> pressedButtonIDs(8); //Note: This is going to be moved when I add the window class, so ignore this for now. 
	WindowData* winData = nullptr;

	Window::Window()
	{
	}

	void Window::Initialize(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync, bool centerWindow)
	{
		if (winData == nullptr)
			winData = new WindowData;
		else
		{
			Utility::Logger::Error("GL Window already initialized, or WindowData is already constructed.");
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

		winData->window = glfwCreateWindow(winWidth, winHeight, "Hello world!", NULL, NULL);
		if (winData->window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(winData->window);

		//glGetString(GL_VERSION);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	bool Window::HandleWindowMessages(int& exitCodeOut)
	{
		glfwPollEvents();
		if (glfwWindowShouldClose(winData->window))
		{
			exitCodeOut = 0;
			return false;
		}

		return true;
	}

	void Window::PresentFrame()
	{
		glfwSwapBuffers(winData->window);
	}

	void Window::SetWindowEventCallback(const WindowEventCallbackFunc& callbackFunc)
	{
		winData->winEventCallback = callbackFunc;
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
			buttonState = glfwGetKey(winData->window, buttonType);
		}
		else if (deviceType == Input::InputDeviceType::LAG_MOUSE)
		{
			buttonState = glfwGetMouseButton(winData->window, GLFW_MOUSE_BUTTON_LEFT);
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
		
		glfwGetCursorPos(winData->window, &xPosD, &yPosD);
		xPos = static_cast<float>(xPosD);
		yPos = static_cast<float>(yPosD);
	}

	const void* Window::GetWindowData()
	{
		return winData;
	}
}