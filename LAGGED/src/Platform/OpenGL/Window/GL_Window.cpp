#include "Platform/Window.h"

#include "Core/Engine.h"
#include "Utility/Logger.h"

#include "GL/glew.h"

#include "GL_InputEnumConversion.h"

#include "Platform/Base/Renderer/RendererBase.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGuizmo/ImGuizmo.h"

namespace LAG
{
	bool Window::PlatformInitialize()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (glewInit() != GLEW_OK)
		{
			Logger::Critical("Failed to initialize GLEW.");
			return false;
		}

		return true;
	}

	bool Window::ImGuiInitialize()
	{
		//Now that the window is initialized, initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 140");

		return true;
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
			m_IsOpen = false;
		return true;
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
}