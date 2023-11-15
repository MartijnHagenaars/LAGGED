#include "Precomp.h"

#include "Platform/OpenGL/Window/GL_Window.h"
#include "Platform/Base/Window/WindowManager.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Model.h"
#include "Platform/OpenGL/Renderer/GL_Shader.h"
#include "Shaders/OpenGL/GL_DefaultObjectShader.h"

#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/LightComponent.h"

#include "ECS/Systems/CameraSystem.h"

#include <utility>
#include "glm/glm.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

namespace LAG::Renderer
{
	struct RendererData
	{
		Shader* shader = nullptr;
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

		renderData->shader = new Shader(ShaderData::object);

		glEnable(GL_DEPTH_TEST);

		return true;
	}

	bool Shutdown()
	{
		return false;
	}

	void StartFrame()
	{
		glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void DrawOptionsWindow()
	{
		bool isOpen = true;
		ImGui::Begin("Render options");

		ImGui::Text("LAGGED Renderer");
		ImGui::Text(std::string("FPS: " + std::to_string(GetEngine().GetFPS())).c_str());
		ImGui::Text(std::string("Render time: " + std::to_string(renderData->renderTime) + "ms").c_str());
		ImGui::Separator();

		if (ImGui::Checkbox("Enable wireframe", &renderData->showWireframe))
		{
			if (renderData->showWireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		ImGui::Checkbox("Enable lighting", &renderData->useLighting);

		ImGui::End();
	}

	void Render()
	{
		renderData->renderTimer.ResetTimer();

		StartFrame();
		DrawOptionsWindow();

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
				GetResourceManager()->GetResource<Model>(meshComp.meshPath)->Render(meshTransformComp, selectedCameraID, *renderData->shader, lights);
				meshTransformComp.rotation.x = (float)glfwGetTime();
				//meshTransformComp.position.x = sinf((float)glfwGetTime()) * 2.f;
			});

		EndFrame();

		renderData->renderTime = renderData->renderTimer.GetMilliseconds();
	}

	void Clear()
	{

	}
}