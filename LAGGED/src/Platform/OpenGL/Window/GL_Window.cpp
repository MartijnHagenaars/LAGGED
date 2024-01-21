#include "Precomp.h"
#include "GL_Window.h"
#include "Events/EventBase.h"
#include "Platform/Base/Window/WindowManager.h"
#include "Core/Engine.h"

#include "GL_InputEnumConversion.h"

#include "Platform/Base/Renderer/RendererBase.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGuizmo/ImGuizmo.h"

namespace LAG
{
	Window::Window()
		: WindowBase()
	{
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		//glfwTerminate();
	}

	void Window::Initialize(unsigned int winWidth, unsigned int winHeight, bool fullscreen, bool useVSync, bool centerWindow)
	{
		if (m_Initialized)
		{
			Logger::Error("Window already initialized.");
			return;
		}

		if (glfwInit() != GLFW_TRUE)
		{
			Logger::Critical("GLFW failed to initialize.");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_WindowWidth = winWidth, m_WindowHeight = winHeight, m_IsFullscreen = fullscreen, m_UseVSync = useVSync;
		m_WindowHalfWidth = static_cast<unsigned int>(winWidth * 0.5f), m_WindowHalfHeight = static_cast<unsigned int>(winHeight * 0.5f);
		GLFWwindow* sharedWindow = (GetWindowManager()->GetPrimaryWindow() == nullptr) ? NULL : GetWindowManager()->GetPrimaryWindow()->m_Window;

		//Create the window. 
		m_Window = glfwCreateWindow(winWidth, winHeight, "LAGGED Engine", NULL, sharedWindow);
		if (m_Window == nullptr)
		{
			Logger::Critical("Failed to create GLFW window");
			glfwTerminate();
			return;
		}
		glfwSetWindowUserPointer(m_Window, this);
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);
	
		if (glewInit() != GLEW_OK)
		{
			Logger::Critical("Failed to initialize GLEW.");
			return;
		}

		//Not sure if necessary
		glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_TRUE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);

		SetupCallbackFunctions();
		pressedButtonIDs.reserve(8);

		//Now that the window is initialized, initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 140");

		//Window is ready to be used! Set initialized to true!
		m_Initialized = true;
	}

	void Window::SetupCallbackFunctions()
	{
		//Setup window focus callback
		auto windowFocusCallback = [](GLFWwindow* winPtr, int focused)
		{
			Window* window = static_cast<LAG::Window*>(glfwGetWindowUserPointer(winPtr));
			if(GetWindowManager()->GetFocussedWindow() != window)
				GetWindowManager()->SetFocussedWindow(window);
		};
		glfwSetWindowFocusCallback(m_Window, windowFocusCallback);

		//Setup window resize callback
		auto windowResizeCallback = [](GLFWwindow* winPtr, int width, int height)
		{
			if (width > 0 && height > 0)
			{
				Window* window = static_cast<LAG::Window*>(glfwGetWindowUserPointer(winPtr));
				window->m_WindowWidth = width;
				window->m_WindowHeight = height;
				window->m_WindowHalfWidth = static_cast<unsigned int>(width * 0.5f);
				window->m_WindowHalfHeight = static_cast<unsigned int>(height * 0.5f);

				glfwMakeContextCurrent(window->m_Window);
				glViewport(0, 0, width, height);

			}
		};
		glfwSetWindowSizeCallback(m_Window, windowResizeCallback);
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

		if (glfwWindowShouldClose(m_Window))
		{
			printf("INJECTED\n");
		}
	}

	void Window::PresentFrame()
	{
		glfwMakeContextCurrent(m_Window);

		LAG::Renderer::Render();
		glfwSwapBuffers(m_Window);
	}

	bool Window::HandleWindowMessages()
	{
		glfwPollEvents();
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
		//glfwMakeContextCurrent(m_Window);

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
		//glfwMakeContextCurrent(m_Window);

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