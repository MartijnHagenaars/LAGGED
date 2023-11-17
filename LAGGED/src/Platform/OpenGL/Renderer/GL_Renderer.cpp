#include "Precomp.h"

#include "Platform/OpenGL/Window/GL_Window.h"
#include "Platform/Base/Window/WindowManager.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Model.h"
#include "Core/Resources/Shader.h"

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

		unsigned int m_FrameBuffer = 0;
		unsigned int m_ColorBuffer = 0;
		unsigned int m_DepthStencilBuffer = 0;

		bool showWireframe = false;
		bool useLighting = true;

		float renderTime = 0.f;
		LAG::Utility::Timer renderTimer;
	};
	RendererData* renderData = nullptr;


	//Initialization functions
	bool CreateFrameBuffer(unsigned int& frameBufferID, unsigned int& renderBufferID, unsigned int& colorBufferID);

	bool Initialize()
	{
		if (renderData != nullptr)
		{
			Utility::Logger::Error("Renderer already initialized.");
			return false;
		}
		renderData = new RendererData();

		//Create the frame buffer, as well as the depth-stencil buffer and the color buffer.
		if (!CreateFrameBuffer(renderData->m_FrameBuffer, renderData->m_DepthStencilBuffer, renderData->m_ColorBuffer))
			return false;

		GetResourceManager()->AddResource<Shader>(Utility::String("res/Shaders/OpenGL/ObjectShader"));
		GetResourceManager()->AddResource<Shader>(Utility::String("res/Shaders/OpenGL/PlaneShader"));

		glEnable(GL_DEPTH_TEST);

		renderData->plane = new Plane();
		renderData->plane->Reload();

		return true;
	}

	bool Shutdown()
	{
		return false;
	}


	bool CreateFrameBuffer(unsigned int& frameBufferID, unsigned int& depthStencilBufferID, unsigned int& colorBufferID)
	{
		//Create the frame buffer
		LAG_GRAPHICS_EXCEPTION(glGenFramebuffers(1, &frameBufferID));
		LAG_GRAPHICS_EXCEPTION(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID));

		//Create a render buffer object for the depth stencil
		LAG_GRAPHICS_EXCEPTION(glGenRenderbuffers(1, &depthStencilBufferID));
		LAG_GRAPHICS_EXCEPTION(glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBufferID));
		LAG_GRAPHICS_EXCEPTION(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GetWindowManager()->GetPrimaryWindow()->GetWidth(), GetWindowManager()->GetPrimaryWindow()->GetHeight()));
		LAG_GRAPHICS_EXCEPTION(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBufferID));

		LAG_GRAPHICS_EXCEPTION(glGenTextures(1, &colorBufferID));
		LAG_GRAPHICS_EXCEPTION(glBindTexture(GL_TEXTURE_2D, colorBufferID));
		LAG_GRAPHICS_EXCEPTION(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetWindowManager()->GetPrimaryWindow()->GetWidth(), GetWindowManager()->GetPrimaryWindow()->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferID, 0);

		bool succeeded = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		if (!succeeded)
			Utility::Logger::Critical("Failed to create frame buffer.");
		else glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return succeeded;
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
				GetResourceManager()->GetResource<Model>(meshComp.meshPath)->Render(meshTransformComp, selectedCameraID, *GetResourceManager()->GetResource<Shader>(Utility::String("res/Shaders/OpenGL/ObjectShader")), lights);

				TransformComponent planeTransform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(75.f));
				renderData->plane->Render(planeTransform, selectedCameraID, *GetResourceManager()->GetResource<Shader>(Utility::String("res/Shaders/OpenGL/PlaneShader")));
			});

		EndFrame();

		renderData->renderTime = renderData->renderTimer.GetMilliseconds();
	}

	void Clear()
	{

	}
}