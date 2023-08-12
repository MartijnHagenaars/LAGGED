#include "Precomp.h"
#include "GL_Renderer.h"

#include "Platform/OpenGL/Window/GL_Window.h"
#include "Platform/OpenGL/Renderer/GL_Shader.h"
#include "Shaders/OpenGL/ObjectShader.h"

namespace LAG::Renderer
{
	//Maybe I don't need this anymore here? 
	struct RendererData
	{
		//const Window::WindowData* winData = nullptr;
		unsigned int VBO = 0;
		unsigned int EBO = 0; 
		unsigned int VAO = 0; 

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

		int shaderCompileStatus = 0;
		float vertices[] = {
			 0.5f,  0.5f, 0.0f, 
			 0.5f, -0.5f, 0.0f, 
			-0.5f, -0.5f, 0.0f, 
			-0.5f,  0.5f, 0.0f  
		};

		unsigned int indices[] = {
			0, 1, 3,   
			1, 2, 3    
		};

		renderData->shader = new Shader(ShaderData::object);


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
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

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
		glBindVertexArray(renderData->VAO);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Not really necessary for improved renderers, but just good for debugging for now
		glBindVertexArray(0); 
	}

	void Clear()
	{

	}
}