#include "Platform/OpenGL/Window/GL_Window.h"
#include "Platform/Base/Window/WindowManager.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Model.h"
#include "Core/Resources/Shader.h"

#include "Platform/OpenGL/Renderer/GL_FrameBuffer.h"
#include "Platform/OpenGL/Renderer/Exceptions/GL_GraphicsExceptionMacros.h"

#include "ECS/Entity.h"
#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/CameraComponent.h"

#include "ECS/Systems/CameraSystem.h"

#include <utility>
#include "glm/glm.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGuizmo/ImGuizmo.h"

#include "Utility/Timer.h"

#include "Core/Resources/Surface.h"

#include "Editor/EditorLayout.h"

namespace LAG::Renderer
{
	struct RendererData
	{
		Surface* testSurface = nullptr;
		Surface* floorSurface = nullptr;
		FrameBuffer* frameBuffer = nullptr;

		EditorLayout* editorLayout = nullptr;

		bool showWireframe = false;
		bool useLighting = true;

		float renderTime = 0.f;
		LAG::Timer renderTimer;
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
		renderData->frameBuffer = new FrameBuffer();

		renderData->editorLayout = new EditorLayout();
		renderData->editorLayout->Initialize();

		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader"));
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader"));

		glEnable(GL_DEPTH_TEST);

		//renderData->surface = new Surface();
		renderData->testSurface = new Surface("res/Assets/Textures/face.png");
		renderData->testSurface->GenerateSurface(0, 0, 512, 512, 0.02f, 25.f, 1234);
		renderData->testSurface->Reload();

		//renderData->floorSurface = new Surface();
		//renderData->floorSurface->GenerateSurface(0, 0, 16, 16, 0.2f, 1337);
		//renderData->floorSurface->Reload();

		renderData->editorLayout = new EditorLayout();
		renderData->editorLayout->Initialize();

		return true;
	}

	bool Shutdown()
	{
		//TODO: Proper cleanup

		return false;
	}

	void ImGuiFrameStart()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		ImGui::ShowDemoWindow();
	}

	void ImGuiFrameEnd()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void DrawOptionsWindow()
	{
		ImGui::Begin("Render options");

		ImGui::Text("LAGGED Renderer");
		ImGui::Text(std::string("FPS: " + std::to_string(GetEngine().GetFPS())).c_str());
		ImGui::Text(std::string("Render time: " + std::to_string(renderData->renderTime) + "ms").c_str());
		ImGui::Separator();

		ImGui::Checkbox("Enable wireframe", &renderData->showWireframe);

		ImGui::Checkbox("Enable lighting", &renderData->useLighting);

		ImGui::End();
	}

	void Render()
	{
		//Start timer for measuring render time
		renderData->renderTimer.ResetTimer();


		// Begin of ImGui rendering
		ImGuiFrameStart();

		//Render ImGui editor windows
		DrawOptionsWindow();
		//renderData->testSurface->DrawDebugWindow();

		renderData->frameBuffer->DrawPostProcessWindow();
		renderData->editorLayout->Render();

		//First render pass using custom frame buffer
		renderData->frameBuffer->FrameStart(renderData->showWireframe);

		//Render all meshes
		GetScene()->Loop<MeshComponent, TransformComponent>([](Entity entity, MeshComponent& meshComp, TransformComponent& meshTransformComp)
			{
				std::vector<std::pair<TransformComponent*, LightComponent*>> lights;

				if (renderData->useLighting)
				{
					lights.reserve(3);
					GetScene()->Loop<LightComponent, TransformComponent>([&meshTransformComp, &lights](Entity entity, LightComponent& lightComp, TransformComponent& lightTransformComp)
						{
							if (lights.size() < TOTAL_POINT_LIGHTS)
								lights.push_back({ &lightTransformComp, &lightComp });
						});
				}

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
				GetResourceManager()->GetResource<Model>(meshComp.meshPath)->Render(meshTransformComp, &selectedCamera, *GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader")), lights);

				TransformComponent testPlaneTransform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f));
				renderData->testSurface->Render(testPlaneTransform, &selectedCamera, *GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader")), lights);

				//TransformComponent floorPlaneTransform(glm::vec3(0.f, -2.f, 0.f), glm::vec3(0.f), glm::vec3(75.f));
				//renderData->floorSurface->Render(floorPlaneTransform, selectedCameraID, *GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/PlaneShader")));
			});

		renderData->frameBuffer->FrameEnd();
		ImGuiFrameEnd();

		renderData->renderTime = renderData->renderTimer.GetMilliseconds();
	}

	void Clear()
	{

	}
}