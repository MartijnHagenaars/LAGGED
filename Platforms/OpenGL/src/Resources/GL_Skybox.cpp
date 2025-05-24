#include "Platform/Resources/Skybox.h"

#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "ECS/Scene.h"
#include "ECS/Entity.h" //FIXME: This include directive shouldn't be necessary
#include "ECS/Components/CameraComponent.h"

#include "Platform/Resources/Shader.h"


namespace LAG
{
	const HashedString skyboxShader = HashedString("res/Shaders/OpenGL/Skybox");

	void Skybox::Load()
	{
		//Generate the cube used for the skybox

		std::vector<glm::vec3> vertices =
		{
			{-1.0f, -1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},
			{-1.0f, -1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f}
		};
		
		std::vector<unsigned int> indices =
		{
			0, 1, 2, 0, 2, 3,
			4, 6, 5, 4, 7, 6,
			4, 0, 3, 4, 3, 7,
			1, 5, 6, 1, 6, 2,
			3, 2, 6, 3, 6, 7,
			4, 5, 1, 4, 1, 0
		};

		VertexBuffer vb;
		vb.SetLayout(BufferLayout({ BufferLayoutElement(BufferVariableType::Int3, false)}));
		vb.SetVertexData(&vertices.data()[0], vertices.size() * sizeof(glm::vec3));

		IndexBuffer ib;
		ib.SetIndexData(indices);
		m_Buffer.Initialize(vb, ib);

		// Create the skybox shader
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/Skybox"));
	}

	void Skybox::Unload()
	{
		m_Buffer.Shutdown();
	}

	void Skybox::SetCubemap(const HashedString& path)
	{
		ResourceManager* pResources = GetResourceManager();
		if (!pResources->Contains(path))
			GetResourceManager()->AddResource<Cubemap>(path);

		m_Cubemap = pResources->GetResource<Cubemap>(path);
	}

	void Skybox::Render(EntityID camEntityID)
	{
		Shader* pShader = GetResourceManager()->GetResource<Shader>(skyboxShader.GetValue());
		CameraComponent* pCamera = GetScene()->GetComponent<CameraComponent>(camEntityID);

		if (!pShader || !pCamera || !m_Cubemap)
		{
			CRITICAL("Failed to render skybox: invalid pointer.");
			return;
		}

		glDepthMask(false);

		glm::mat4 skyboxViewMat = glm::mat4(glm::mat3(pCamera->viewMat));
		pShader->Bind();
		pShader->SetMat4("a_ViewMat", skyboxViewMat);
		pShader->SetMat4("a_ProjMat", pCamera->projMat);

		m_Cubemap->Bind();
		m_Buffer.Render();

		glDepthMask(true);
	}
}