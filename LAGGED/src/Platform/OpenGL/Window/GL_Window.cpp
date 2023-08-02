#include "Precomp.h"
#include "GL_Window.h"

#include "GL_InputEnumConversion.h"
#include <unordered_set>

namespace LAG::Window
{
	std::unordered_set<size_t> pressedButtonIDs(8); //Note: This is going to be moved when I add the window class, so ignore this for now. 
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

		return true;
	}
	
	void Shutdown()
	{
		glfwTerminate();
	}

	bool HandleWindowMessages(int& exitCodeOut)
	{
		glfwPollEvents();

		if (glfwWindowShouldClose(winData->window))
		{
			exitCodeOut = 0;
			return false;
		}
		else return true;
	}

	void SetWindowEventCallback(const WindowEventCallbackFunc& callbackFunc)
	{
		winData->winEventCallback = callbackFunc;
	}

	void Update()
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

	bool CheckButtonPress(const Input::InputActionData& inputType, bool onlyDetectSinglePress)
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

	const void* GetWindowData()
	{
		return winData;
	}
}