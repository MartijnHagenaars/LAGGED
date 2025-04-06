#include "Platform/RenderBackend.h"

#include "Utility/DebugLine.h"

#include "Resources/Model.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/Surface.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"

#include "ECS/Scene.h"
#include "ECS/Entity.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/ModelComponent.h"
#include "ECS/Components/TerrainComponents.h"
#include "ECS/Systems/BasicSystems.h"
#include "ECS/Systems/CameraSystem.h"


#include "Platform/Window.h"
#include "Platform/Resources/Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
		GetWindow()->SetResizeCallBack(std::bind(&Renderer::OnResize, this, std::placeholders::_1, std::placeholders::_2));

		return ErrResult::SUCCESS;
	}

	ErrResult Renderer::Shutdown()
	{
		DebugLine::Shutdown();

		return ErrResult::SUCCESS;
	}

	void Renderer::DrawDebugLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
	{
		DebugLine::AddLineToQueue(p1, p2, color);
	}

	Model* Renderer::GetModel()
	{
		return nullptr;
	}

	Shader* Renderer::GetShader()
	{
		return nullptr;
	}

	Texture* Renderer::GetTexture()
	{
		return nullptr;
	}

	Surface Renderer::GetSurface()
	{
		return nullptr;
	}

	void Renderer::PresentFrame()
	{
		//Start timer for measuring render time
		m_RenderTimer.ResetTimer();

		//// Begin of ImGui rendering
		//ImGuiFrameStart();

		////Render ImGui editor windows
		//DrawOptionsWindow();

		//GetToolsManager()->Render();

		//First render pass using custom frame buffer
		EntityID camEntityID = CameraSystem::GetActiveCameraEntityID();
		if (camEntityID == ENTITY_NULL)
			LAG_ASSERT("Camera Entity ID is nullptr.");

		CameraComponent* camComp = GetScene()->GetComponent<CameraComponent>(camEntityID);
		if (camComp == nullptr)
			LAG_ASSERT("Tried to get CameraComponent using Camera Entity ID, but failed to retrieve CameraComponent pointer.");

		camComp->framebuffer->FrameStart(m_Properties.showWireframe);

		//Get an active camera
		CameraSystem::Update(camEntityID);

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

		DebugLine::PresentDebugLines();

		camComp->framebuffer->FrameEnd();

		//ImGuiFrameEnd();
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

		camComp->framebuffer->Resize(camComp->framebuffer->GetSize());
		camComp->hasCameraDimensionChanged = true;
	}


	Model* LAG::GetModel()
	{
		return nullptr;
	}

}