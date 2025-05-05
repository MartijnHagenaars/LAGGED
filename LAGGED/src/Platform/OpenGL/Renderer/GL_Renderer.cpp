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

#include "GL_Cubemap.h"
#include "GL_Skybox.h"

namespace LAG
{
	static Cubemap* skyCubemap = nullptr;
	static Skybox* skybox = nullptr;

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
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/Skybox"));

		//skyCubemap = new Cubemap(FileIO::Directory::Assets, "Cubemaps/Desert");
		skyCubemap = new Cubemap(FileIO::Directory::Assets, "Cubemaps/Sky_2k");
		skyCubemap->Load();

		skybox = new Skybox();
		skybox->Load();

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
	{
		//Start timer for measuring render time
		m_RenderTimer.ResetTimer();

		// Begin of ImGui rendering
		ImGuiFrameStart();

		//Render ImGui editor windows
		DrawOptionsWindow();

		GetToolsManager()->Render();

		//First render pass using custom frame buffer
		EntityID camEntityID = CameraSystem::GetActiveCameraEntityID();
		if (camEntityID == ENTITY_NULL)
			LAG_ASSERT("Camera Entity ID is nullptr.");

		CameraSystem::Update(camEntityID);
		CameraComponent* camComp = GetScene()->GetComponent<CameraComponent>(camEntityID);
		if(camComp == nullptr)
			LAG_ASSERT("Tried to get CameraComponent using Camera Entity ID, but failed to retrieve CameraComponent pointer.");

		camComp->framebuffer->FrameStart(m_ShowWireframe);


		//Temp skybox code
		{
			//TODO: Skybox is rendering black / without a texture. What is causing this?
			Shader* skyShader = GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/Skybox"));
			skyShader->Bind();

			glm::mat4 skyboxViewMat = glm::mat4(glm::mat3(camComp->viewMat));
			skyShader->SetMat4("a_ViewMat", skyboxViewMat);
			skyShader->SetMat4("a_ProjMat", camComp->projMat);
			skybox->Render(*skyCubemap);
		}

		//Get some lights
		//TODO: Should be redone. Doesn't allow for more than three lights
		std::vector<std::pair<TransformComponent*, LightComponent*>> lights;
		if (m_UseLighting)
		{
			lights.reserve(3);
			GetScene()->RunSystem<LightComponent, TransformComponent>([&lights](EntityID entity, LightComponent* lightComp, TransformComponent* lightTransformComp)
				{
					if (lights.size() < TOTAL_POINT_LIGHTS)
						lights.push_back({ lightTransformComp, lightComp });
				});
		}

		const auto& renderEntities = GetScene()->GetEntitiesWithComponents<TransformComponent>();
		for (const auto& entityID : renderEntities)
		{
			DefaultComponent* defPtr = GetScene()->GetComponent<DefaultComponent>(entityID);
			TransformComponent* transformPtr = GetScene()->GetComponent<TransformComponent>(entityID);

			ModelComponent* modelPtr = GetScene()->GetComponent<ModelComponent>(entityID);
			if (modelPtr != nullptr)
			{
				//TODO: Really ugly approach. meshComp.modelHandle.m_ModelLookup should be shortened to meshComp.modelHandle;
				if (defPtr->visible && modelPtr->modelHandle.m_ModelLookup.GetValue() != 0)
				{
					GetResourceManager()->GetResource<Model>(modelPtr->modelHandle.m_ModelLookup)->Render(
						entityID, camEntityID,
						*GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader")),
						lights);
				}
			}

			SurfaceComponent* surfacePtr = GetScene()->GetComponent<SurfaceComponent>(entityID);
			if (surfacePtr)
			{
				if (defPtr->visible)
					surfacePtr->surface.Render(entityID, camEntityID, *GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader")), lights);
			}

			ProceduralSurfaceComponent* procSurfacePtr = GetScene()->GetComponent<ProceduralSurfaceComponent>(entityID);
			if (procSurfacePtr)
			{
				if (defPtr->visible)
					procSurfacePtr->surface.Render(entityID, camEntityID, *GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader")), lights);
			}
		}

		//Render all lines in the line render list
		if (!m_LineRenderList.empty())
		{
			for (const auto& it : m_LineRenderList)
				LineTool::DrawLine(it.pos1, it.pos2, it.color);
			m_LineRenderList.clear();
		}

		camComp->framebuffer->FrameEnd();

		ImGuiFrameEnd();
		m_RenderTime = m_RenderTimer.GetMilliseconds();
	}
}