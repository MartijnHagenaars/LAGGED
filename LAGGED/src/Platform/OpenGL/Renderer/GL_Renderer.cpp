#include "Precomp.h"
#include "GL_Renderer.h"

#include "Platform/OpenGL/Window/GL_Window.h"
#include "Platform/Base/Window/WindowManager.h"

#include "Platform/OpenGL/Renderer/GL_Texture.h"
#include "Platform/OpenGL/Renderer/GL_Shader.h"
#include "Shaders/OpenGL/GL_DefaultObjectShader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "json/json.hpp"

namespace LAG::Renderer
{
	//Maybe I don't need this anymore here? 
	struct RendererData
	{
		//const Window::WindowData* winData = nullptr;
		unsigned int VBO = 0;
		unsigned int EBO = 0; 
		unsigned int VAO = 0; 

		glm::vec3 objectPos = glm::vec3(0.f, 0.f, 1.f);
		glm::vec3 objectRot = glm::vec3(0.f);

		glm::vec3 cameraPos = glm::vec3(0.f, 0.f, -2.f);
		glm::vec3 cameraRot = glm::vec3(0.f);

		Shader* shader = nullptr;
		Texture* texture = nullptr;
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

		int shaderCompileStatus = 0;
		float vertices[] = {
			//Vertex positions		//Tex coords
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 3,   
			1, 2, 3    
		};

		renderData->shader = new Shader(ShaderData::object);
		renderData->texture = new Texture("res/Assets/Textures/corndog.png");

		//Create the VAO
		glGenVertexArrays(1, &renderData->VAO); //What's the difference between this func and "glCreateVertexArrays"?
		glBindVertexArray(renderData->VAO);

		//Create the VBO
		glGenBuffers(1, &renderData->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, renderData->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//Create the EBO (element buffer object / index buffer)
		glGenBuffers(1, &renderData->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderData->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glUseProgram(0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
		glClear(GL_COLOR_BUFFER_BIT);

		renderData->shader->Bind();
		renderData->texture->Bind();
		renderData->shader->SetInt("texture1", 0); //Testing, remove

		glm::mat4 transformation = glm::mat4(1.0f);
		transformation = glm::rotate(transformation, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		transformation = glm::translate(transformation, glm::vec3(0.5f, -0.5f, 0.0f));
		renderData->shader->SetMat4("transformMat", transformation);


		//Do transformation calculations
		//First, calculate the model matrix
		glm::mat4 modelMat = glm::mat4(1.f);
		modelMat = glm::translate(modelMat, renderData->objectPos);
		modelMat = glm::rotate(modelMat, glm::radians(-45.f), glm::vec3(1.f, 0.f, 0.f));
		
		//Next, the view matrix
		glm::mat4 viewMat = glm::mat4(1.f);
		viewMat = glm::translate(viewMat, renderData->cameraPos);
		
		//Lastly, the projection matrix
		glm::mat4 projectionMat = glm::mat4(1.f);
		projectionMat = glm::perspective(glm::radians(45.f), static_cast<float>(WindowManager::Get().GetFocussedWindow()->GetWidth() / WindowManager::Get().GetFocussedWindow()->GetHeight()), 0.1f, 100.f);

		//Send over the matrices to the shader
		renderData->shader->SetMat4("modelMat", modelMat);
		renderData->shader->SetMat4("viewMat", viewMat);
		renderData->shader->SetMat4("projMat", projectionMat);

		glBindVertexArray(renderData->VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Not really necessary for improved renderers, but just good for debugging for now
		glBindVertexArray(0); 
	}

	void Clear()
	{

	}
}