#include "Precomp.h"
//#include "GL_Renderer.h"

#include "Platform/OpenGL/Window/GL_Window.h"
#include "Platform/Base/Window/WindowManager.h"

#include "Core/Resources/ResourceManager.h"
#include "Platform/OpenGL/Renderer/GL_Texture.h"
#include "Platform/OpenGL/Renderer/GL_Shader.h"
#include "Platform/OpenGL/Renderer/GL_Model.h"
#include "Shaders/OpenGL/GL_DefaultObjectShader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "json/json.hpp"

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

		//ResourceManager::Get().AddResource<Texture>(Utility::String("res/Assets/Textures/corndog.png"));
		ResourceManager::Get().AddResource<Model>(Utility::String("res/Assets/Models/Cube/Cube.gltf"));

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
		glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Model* modelThingy = ResourceManager::Get().GetResource<Model>(Utility::String("res/Assets/Models/Cube/Cube.gltf"));
		modelThingy->Render(*renderData->shader);

		auto help = glGetError();

		//Not really necessary for improved renderers, but just good for debugging for now
		glBindVertexArray(0); 
	}

	void Clear()
	{

	}
}