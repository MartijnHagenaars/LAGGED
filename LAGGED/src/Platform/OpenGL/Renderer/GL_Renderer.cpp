#include "Platform/Renderer.h"

#include "Core/Engine.h"
#include "Platform/Window.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Model.h"
#include "Core/Resources/Shader.h"

#include "Platform/OpenGL/Renderer/GL_LineTool.h"

#include "GL_FrameBuffer.h"
#include "GL_ErrorChecks.h"

#include "ECS/Entity.h"
#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/MeshComponent.h"
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
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	bool Renderer::Initialize()
	{
		//Create some essential shaders.
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader"));
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader"));

		glEnable(GL_DEPTH_TEST);

		LineTool::Initialize();

		//Setup resize callback
		GetWindow()->SetResizeCallBack(std::bind(&Renderer::OnResize, this, std::placeholders::_1, std::placeholders::_2));

		return true;
	}

	bool Renderer::Shutdown()
	{
		LineTool::Shutdown();

		return true;
	}

	void Renderer::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
	{
		m_LineRenderList.emplace_back(LineData{ p1, p2, color });
	}

	void Renderer::OnResize(unsigned int width, unsigned int height)
	{
		Logger::Info("Window resize: {0}, {1}", width, height);

		glViewport(0, 0, width, height);
		CameraSystem::ResizeCameraBuffers();
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
	{
		//Start timer for measuring render time
		m_RenderTimer.ResetTimer();

		// Begin of ImGui rendering
		ImGuiFrameStart();

		//Render ImGui editor windows
		DrawOptionsWindow();

		GetToolsManager()->Render();

		//First render pass using custom frame buffer
		CameraSystem::GetActiveCameraEntity().GetComponent<CameraComponent>()->framebuffer->FrameStart(m_ShowWireframe);


		//Get an active camera
		//TODO: This should be done in some sort of camera system. We shouldn't have to loop through the entire scene every single time to find a camera
		Entity selectedCamera = CameraSystem::GetActiveCameraEntity();
		CameraSystem::Update(&selectedCamera);

		//Get some lights
		//TODO: Should be redone. Doesn't allow for more than three lights
		std::vector<std::pair<TransformComponent*, LightComponent*>> lights;
		if (m_UseLighting)
		{
			lights.reserve(3);
			GetScene()->Loop<LightComponent, TransformComponent>([&lights](Entity entity, LightComponent& lightComp, TransformComponent& lightTransformComp)
				{
					if (lights.size() < TOTAL_POINT_LIGHTS)
						lights.push_back({ &lightTransformComp, &lightComp });
				});
		}

		//Render all meshes
		GetScene()->Loop<MeshComponent, TransformComponent>([&selectedCamera, &lights](Entity entity, MeshComponent& meshComp, TransformComponent& transformComp)
			{
				if (entity.GetComponent<DefaultComponent>()->visible)
				{
					GetResourceManager()->GetResource<Model>(meshComp.meshPath)->Render(
						transformComp, &selectedCamera,
						*GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader")),
						lights);
				}
			});

		//Render all surfaces
		GetScene()->Loop<SurfaceComponent, TransformComponent>([&selectedCamera, &lights](Entity entity, SurfaceComponent& surfaceComp, TransformComponent& transformComp)
			{
				if (entity.GetComponent<DefaultComponent>()->visible)
					surfaceComp.surface.Render(transformComp, &selectedCamera, *GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader")), lights);
			});
		GetScene()->Loop<ProceduralSurfaceComponent, TransformComponent>([&selectedCamera, &lights](Entity entity, ProceduralSurfaceComponent& surfaceComp, TransformComponent& transformComp)
			{
				if (entity.GetComponent<DefaultComponent>()->visible)
					surfaceComp.surface.Render(transformComp, &selectedCamera, *GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader")), lights);
			});

		//Render all lines in the line render list
		if (!m_LineRenderList.empty())
		{
			for (const auto& it : m_LineRenderList)
				LineTool::DrawLine(it.pos1, it.pos2, it.color);
			m_LineRenderList.clear();
		}

		CameraSystem::GetActiveCameraComponent()->framebuffer->FrameEnd();

		ImGuiFrameEnd();
		m_RenderTime = m_RenderTimer.GetMilliseconds();
	}
}