#include "Precomp.h"

#include "Platform/OpenGL/Window/GL_Window.h"
#include "Platform/Base/Window/WindowManager.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Model.h"
#include "Core/Resources/Shader.h"

#include "Platform/OpenGL/Renderer/GL_FrameBuffer.h"
#include "Platform/OpenGL/Renderer/Exceptions/GL_GraphicsExceptionMacros.h"

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

#include "Utility/Timer.h"

#include "Core/Resources/Plane.h"

namespace LAG::Renderer
{
	struct RendererData
	{
		Plane* plane = nullptr;
		FrameBuffer* frameBuffer = nullptr;

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

		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader"));
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/PlaneShader"));

		glEnable(GL_DEPTH_TEST);

		renderData->plane = new Plane();
		renderData->plane->Reload();

		return true;
	}

	bool Shutdown()
	{
		return false;
	}

	void ImGuiFrameStart()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
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
		renderData->renderTimer.ResetTimer();

		ImGuiFrameStart();
		DrawOptionsWindow();
		renderData->frameBuffer->DrawPostProcessWindow();

		//First render pass using custom frame buffer
		renderData->frameBuffer->FrameStart(renderData->showWireframe);

		//Render all meshes
		GetScene()->Loop<MeshComponent, TransformComponent>([](uint32_t entityID, MeshComponent& meshComp, TransformComponent& meshTransformComp)
			{
				std::vector<std::pair<TransformComponent*, LightComponent*>> lights;

				if (renderData->useLighting)
				{
					lights.reserve(3);
					GetScene()->Loop<LightComponent, TransformComponent>([&meshTransformComp, &lights](uint32_t entityID, LightComponent& lightComp, TransformComponent& lightTransformComp)
						{
							if (lights.size() < TOTAL_POINT_LIGHTS)
								lights.push_back({ &lightTransformComp, &lightComp });
						});
				}

				bool doesCameraExist;
				uint32_t selectedCameraID;
				doesCameraExist = GetScene()->Loop<CameraComponent, TransformComponent>([&selectedCameraID](uint32_t entityID, CameraComponent& camera, TransformComponent& transform)
					{
						if (camera.isActive)
						{
							selectedCameraID = entityID;
						}
					});

				//Don't render anything if there isn't a valid camera.
				if (!doesCameraExist)
					return;

				CameraSystem::Update(selectedCameraID);
				GetResourceManager()->GetResource<Model>(meshComp.meshPath)->Render(meshTransformComp, selectedCameraID, *GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader")), lights);

				//TransformComponent planeTransform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(75.f));
				//renderData->plane->Render(planeTransform, selectedCameraID, *GetResourceManager()->GetResource<Shader>(Utility::String("res/Shaders/OpenGL/PlaneShader")));
			});

		renderData->frameBuffer->FrameEnd();
		ImGuiFrameEnd();

		renderData->renderTime = renderData->renderTimer.GetMilliseconds();
	}

	void Clear()
	{

	}
}