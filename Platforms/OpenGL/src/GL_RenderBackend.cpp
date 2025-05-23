#include "Platform/RenderBackend.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <../imgui/ImGui/imgui.h>
#include <../imgui/ImGui/imgui_impl_glfw.h>
#include <../imgui/ImGui/imgui_impl_opengl3.h>
#include <../imgui/ImGuizmo/ImGuizmo.h>

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"

#include "Platform/Resources/Model.h"
#include "Platform/Resources/Shader.h"
#include "Platform/Resources/Texture.h"
#include "Platform/Resources/Surface.h"
#include "Platform/Resources/Skybox.h"
#include "Platform/Resources/Cubemap.h"

#include "Platform/Window.h"
#include "Platform/Resources/Shader.h"

#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/ModelComponent.h"
#include "ECS/Components/TerrainComponents.h"
#include "ECS/Systems/BasicSystems.h"
#include "ECS/Systems/CameraSystem.h"

#include "Utility/GL_DebugLine.h"
#include "Utility/GL_ErrorCodeLookup.h"

namespace LAG
{
	ErrResult Renderer::Initialize()
	{
		//Create some essential shaders.
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader"));
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader"));

		glEnable(GL_DEPTH_TEST);

		DebugLine::Initialize();

		//Setup resize callback
		GetWindow()->SetResizeCallBack(&LAG::Renderer::OnResize);

		//Setup OpenGL debug message callback
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
			{
				if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
					ERROR("OpenGL Error: {0} Type: {1}, Severity: {2}, Message: {3}", type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "", ConvertErrorTypeToString(type), ConvertErrorSeverityToString(severity), message);
			}, 0);

		return ErrResult::SUCCESS;
	}

	ErrResult Renderer::Shutdown()
	{
		DebugLine::Shutdown();
		if (m_Skybox)
			delete m_Skybox;
		m_Skybox = nullptr;

		return ErrResult::SUCCESS;
	}

	void LAG::Renderer::SetSkyboxCubemap(const HashedString& path)
	{
		if (!m_Skybox)
		{
			m_Skybox = new Skybox;
			m_Skybox->Load();
		}
		m_Skybox->SetCubemap(path);
	}

	void Renderer::DrawDebugLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
	{
		DebugLine::AddLineToQueue(p1, p2, color);
	}

	void ImGuiFrameStart()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiFrameEnd()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLFWwindow* prePlatformUpdateContext = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(prePlatformUpdateContext);
	}

	void Renderer::PresentFrame()
	{
		//Start timer for measuring render time
		m_RenderTimer.ResetTimer();

		//First render pass using custom frame buffer
		EntityID camEntityID = CameraSystem::GetActiveCameraEntityID();
		if (camEntityID == ENTITY_NULL)
			LAG_ASSERT("Camera Entity ID is nullptr.");

		CameraSystem::Update(camEntityID);
		CameraComponent* camComp = GetScene()->GetComponent<CameraComponent>(camEntityID);
		if (camComp == nullptr)
			LAG_ASSERT("Tried to get CameraComponent using Camera Entity ID, but failed to retrieve CameraComponent pointer.");

		camComp->frameBuffer->FrameStart(m_Properties.showWireframe);

		//Get some lights
		//TODO: Should be redone. Doesn't allow for more than three lights
		std::vector<std::pair<TransformComponent*, LightComponent*>> lights;
		if (m_Properties.useLighting)
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

		m_Skybox->Render(camEntityID);
		DebugLine::PresentDebugLines();

		camComp->frameBuffer->FrameEnd();


		ImGuiFrameStart();
		for (int i = 0; i < m_ImGuiRenderCallbacks.size(); i++)
		{
			m_ImGuiRenderCallbacks[i]();
		}
		ImGuiFrameEnd();

		m_RenderTime = m_RenderTimer.GetMilliseconds();
	}

	void Renderer::OnResize(unsigned int width, unsigned int height)
	{
		INFO("Window resize: {0}, {1}", width, height);

		glViewport(0, 0, width, height);

		EntityID camEntityID = CameraSystem::GetActiveCameraEntityID();
		CameraComponent* camComp = GetScene()->GetComponent<CameraComponent>(camEntityID);
		if (!camComp)
			CRITICAL("Attempting to get CameraComponent from Entity {}, but received nullptr.", camEntityID);

		camComp->frameBuffer->Resize(camComp->frameBuffer->GetSize());
		camComp->hasCameraDimensionChanged = true;
	}

	void Renderer::RegisterImGuiRenderCallback(std::function<void()> func)
	{
		if (func)
			m_ImGuiRenderCallbacks.emplace_back(func);
	}

}