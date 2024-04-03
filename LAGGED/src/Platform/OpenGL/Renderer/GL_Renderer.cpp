#include "Platform/Base/Renderer/RendererBase.h"

#include "Core/Engine.h"
#include "Platform/Window.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Model.h"
#include "Core/Resources/Shader.h"

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

namespace LAG::Renderer
{
	struct RendererData
	{
		bool m_ShowWireframe = false;
		bool m_UseLighting = true;

		LAG::Timer m_RenderTimer;
		float m_RenderTime = 0.f;
	};
	RendererData* renderData = nullptr;

	bool Initialize()
	{
		if (renderData != nullptr)
		{
			Logger::Error("Renderer already initialized.");
			return false;
		}
		renderData = new RendererData();

		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader"));
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader"));

		glEnable(GL_DEPTH_TEST);

		//Setup resize callback
		GetWindow()->SetResizeCallBack(&OnResize);

		return true;
	}

	bool Shutdown()
	{
		//TODO: Proper cleanup

		return false;
	}

	void OnResize(unsigned int width, unsigned int height)
	{
		Logger::Info("Window resize: {0}, {1}", width, height);

		glViewport(0, 0, width, height);
		CameraSystem::ResizeCameraBuffers();
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

	void DrawOptionsWindow()
	{
		ImGui::Begin("Render options");

		ImGui::Text("LAGGED Renderer");
		ImGui::Text(std::string("FPS: " + std::to_string(GetEngine().GetFPS())).c_str());
		ImGui::Text(std::string("Render time: " + std::to_string(renderData->m_RenderTime) + "ms").c_str());
		ImGui::Separator();

		ImGui::Checkbox("Enable wireframe", &renderData->m_ShowWireframe);

		ImGui::Checkbox("Enable lighting", &renderData->m_UseLighting);

		ImGui::End();
	}

	void Render()
	{
		//Start timer for measuring render time
		renderData->m_RenderTimer.ResetTimer();

		// Begin of ImGui rendering
		ImGuiFrameStart();

		//Render ImGui editor windows
		DrawOptionsWindow();

		GetToolsManager()->Render();

		//First render pass using custom frame buffer
		CameraSystem::GetActiveCameraEntity().GetComponent<CameraComponent>()->m_Framebuffer->FrameStart(renderData->m_ShowWireframe);


		//Get an active camera
		//TODO: This should be done in some sort of camera system. We shouldn't have to loop through the entire scene every single time to find a camera
		bool doesCameraExist;
		Entity selectedCamera;
		doesCameraExist = GetScene()->Loop<CameraComponent, TransformComponent>([&selectedCamera](Entity entity, CameraComponent& camera, TransformComponent& transform)
			{
				if (camera.isActive)
				{
					selectedCamera = entity;
				}
			});

		//Don't render anything if there isn't a valid camera.
		if (!doesCameraExist)
			return;
		CameraSystem::Update(&selectedCamera);

		//Get some lights
		//TODO: Should be redone. Doesn't allow for more than three lights
		std::vector<std::pair<TransformComponent*, LightComponent*>> lights;
		if (renderData->m_UseLighting)
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
				GetResourceManager()->GetResource<Model>(meshComp.meshPath)->Render(
					transformComp, &selectedCamera,
					*GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader")),
					lights);
			});

		//Render all surfaces
		GetScene()->Loop<SurfaceComponent, TransformComponent>([&selectedCamera, &lights](Entity entity, SurfaceComponent& surfaceComp, TransformComponent& transformComp)
			{
				surfaceComp.m_Surface->Render(transformComp, &selectedCamera, *GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader")), lights);
			});
		GetScene()->Loop<ProceduralSurfaceComponent, TransformComponent>([&selectedCamera, &lights](Entity entity, ProceduralSurfaceComponent& surfaceComp, TransformComponent& transformComp)
			{
				surfaceComp.m_Surface.Render(transformComp, &selectedCamera, *GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader")), lights);
			});

		CameraSystem::GetActiveCameraEntity().GetComponent<CameraComponent>()->m_Framebuffer->FrameEnd();

		ImGuiFrameEnd();
		renderData->m_RenderTime = renderData->m_RenderTimer.GetMilliseconds();
	}

	void Clear()
	{

	}
}