#include "Platform/Window.h"

#include <../imgui/ImGui/imgui.h>
#include <../imgui/ImGui/imgui_impl_glfw.h>
#include <../imgui/ImGui/imgui_impl_opengl3.h>
#include <../imgui/ImGuizmo/ImGuizmo.h>
#include <../imgui/ImGuiTheme.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Core/Engine.h"
#include "Utility/Logger.h"

#include "Platform/RenderBackend.h"

namespace LAG
{
	bool Window::PlatformInitialize()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (glewInit() != GLEW_OK)
		{
			CRITICAL("Failed to initialize GLEW.");
			return false;
		}

		return true;
	}

	bool Window::ImGuiInitialize()
	{
		// Now that the window is initialized, initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		ApplyImGuiTheme();

		return true;
	}

	void Window::PresentFrame()
	{
		glfwMakeContextCurrent(m_Window);
		GetRenderer()->PresentFrame();
		glfwSwapBuffers(m_Window);
	}
}