#include "Precomp.h"
#include "GL_Renderer.h"

#include "Platform/OpenGL/Window/GL_Window.h"

namespace LAG::Renderer
{
	//Maybe I don't need this anymore here? 
	struct RendererData
	{
		//const Window::WindowData* winData = nullptr;
		unsigned int VBO = 0;
		unsigned int EBO = 0; 
		unsigned int VAO = 0; 

		//unsigned int vertexShader = 0;
		//unsigned int fragmentShader = 0;
		unsigned int shaderProgram = 0; 
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

		//Vertex shader source
		const char* vertexShaderSource = "#version 330 core			\n"
			"layout (location = 0) in vec3 aPos;					\n"
			"void main()											\n"
			"{														\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);	\n"
			"}														\0";

		//Fragment shader source
		const char* fragmentShaderSource = "#version 330 core	\n"
			"	out vec4 FragColor;									\n"
			"														\n"
			"void main()											\n"
			"{														\n"
			"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);			\n"
			"}														\0";

		//Create the vertex shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompileStatus);
		if (shaderCompileStatus != GL_TRUE)
		{
			char shaderCompileStatusInfoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, shaderCompileStatusInfoLog);
			Utility::Logger::Critical("Failed to compile vertex shader: {0}", shaderCompileStatusInfoLog);
			return false;
		}

		//Create the fragment shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompileStatus);
		if (shaderCompileStatus != GL_TRUE)
		{
			char shaderInfoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, shaderInfoLog);
			Utility::Logger::Critical("Failed to compile fragment shader: {0}", shaderInfoLog);
			return false;
		}
		
		//Now create the shader program
		renderData->shaderProgram = glCreateProgram(); 
		glAttachShader(renderData->shaderProgram, vertexShader);
		glAttachShader(renderData->shaderProgram, fragmentShader);
		glLinkProgram(renderData->shaderProgram);

		int shaderLinkStatus = 0;
		glGetProgramiv(renderData->shaderProgram, GL_LINK_STATUS, &shaderLinkStatus);
		if (shaderLinkStatus != GL_TRUE)
		{
			char shaderProgramLog[512];
			glGetProgramInfoLog(renderData->shaderProgram, 512, NULL, shaderProgramLog);
			Utility::Logger::Critical("Failed to link shader program: {0}", shaderProgramLog);
			return false;
		}
		glUseProgram(renderData->shaderProgram);
		
		//Since the shaders have been linked to the program, we can get rid of them. 
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);


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

		glUseProgram(renderData->shaderProgram);
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