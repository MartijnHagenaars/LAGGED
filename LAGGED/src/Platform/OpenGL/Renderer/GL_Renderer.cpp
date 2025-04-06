#include "Platform/Renderer.h"

#include "Core/Engine.h"
#include "Platform/Window.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Core/Resources/ResourceManager.h"
#include "Platform/Resources/Model.h"
#include "Platform/Resources/Shader.h"

#include "Platform/OpenGL/Renderer/GL_LineTool.h"

#include "GL_FrameBuffer.h"
#include "GL_ErrorChecks.h"

#include "ECS/Entity.h"
#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/ModelComponent.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/TerrainComponents.h"

#include "ECS/Systems/CameraSystem.h"

#include "glm/glm.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGuizmo/ImGuizmo.h"

#include "Utility/Timer.h"
#include "Editor/ToolsManager.h"

namespace LAG
{
	void Renderer::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
	{
		m_LineRenderList.emplace_back(LineData{ p1, p2, color });
	}

	void Renderer::OnResize(unsigned int width, unsigned int height)
	{
		INFO("Window resize: {0}, {1}", width, height);

		glViewport(0, 0, width, height);

		// Resize all camera buffers
		GetScene()->RunSystem<CameraComponent>([](EntityID cameraEntity, CameraComponent* cameraComp)
			{
				cameraComp->framebuffer->Resize(cameraComp->framebuffer->GetSize());
				cameraComp->hasCameraDimensionChanged = true;
			});
	}

	void Renderer::ImGuiFrameStart()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void Renderer::ImGuiFrameEnd()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLFWwindow* prePlatformUpdateContext = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(prePlatformUpdateContext);
	}

	//TODO: NEEDS TO BE MOVED!
	void Renderer::DrawOptionsWindow()
	{
		ImGui::Begin("Render options");

		ImGui::Text("LAGGED Renderer");
		ImGui::Text(std::string("FPS: " + std::to_string(GetEngine().GetFPS())).c_str());
		ImGui::Text(std::string("Render time: " + std::to_string(m_RenderTime) + "ms").c_str());
		ImGui::Separator();

		ImGui::Checkbox("Enable wireframe", &m_ShowWireframe);

		ImGui::Checkbox("Enable lighting", &m_UseLighting);

		ImGui::End();
	}

	void Renderer::Render()
	
}