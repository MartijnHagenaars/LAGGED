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

#include <utility>
#include "glm/glm.hpp"

namespace LAG::Renderer
{
	struct RendererData
	{
		Shader* shader = nullptr;
	};
	RendererData* renderData = nullptr;


	bool Initialize()
	{
		if (renderData != nullptr)
		{
			Utility::Logger::Error("Renderer already initialized.");
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

	}

	void EndFrame()
	{

	}

	void Render()
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GetScene()->Loop<MeshComponent, TransformComponent>([](uint32_t entityID, MeshComponent& meshComp, TransformComponent& meshTransformComp)
			{
				std::vector<std::pair<TransformComponent*, LightComponent*>> lights;
				lights.reserve(3);

				GetScene()->Loop<LightComponent, TransformComponent>([&meshTransformComp, &lights](uint32_t entityID, LightComponent& lightComp, TransformComponent& lightTransformComp)
					{
						if (lights.size() < TOTAL_POINT_LIGHTS)
							lights.push_back({ &lightTransformComp, &lightComp });
					});

				GetResourceManager()->GetResource<Model>(meshComp.meshPath)->Render(meshTransformComp, *renderData->shader, lights);
				meshTransformComp.rotation.x = (float)glfwGetTime();
				meshTransformComp.position.x = sinf((float)glfwGetTime()) * 2.f;
			});


		//Not really necessary for improved renderers, but just good for debugging for now
		glBindVertexArray(0); 
	}

	void Clear()
	{

	}
}