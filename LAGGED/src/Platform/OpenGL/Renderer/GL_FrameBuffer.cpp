#include "GL_FrameBuffer.h"

#include "Core/Engine.h"
#include "Platform/Window.h"

#include "Core/Resources/Shader.h"
#include "Core/Resources/ResourceManager.h"

#include "GL/glew.h"
#include "Platform/OpenGL/Renderer/Exceptions/GL_GraphicsExceptionMacros.h"

#include "ImGui/imgui.h"

#include "Utility/Logger.h"

namespace LAG
{
	FrameBuffer::FrameBuffer() : FrameBufferBase()
	{
		if (!Initialize())
			Logger::Critical("Failed to create frame buffer.");
	}

	FrameBuffer::~FrameBuffer()
	{
		if (!Shutdown())
			Logger::Critical("Failed to shut down frame buffer.");
	}

	bool FrameBuffer::Initialize()
	{
		//First, create the quad for displaying the frame buffer
		float vertices[] =
		{
			-1.f, -1.f,	0.f, 0.f,
			 1.f, -1.f,	1.f, 0.f,
			-1.f,  1.f,	0.f, 1.f,
			 1.f,  1.f,	1.f, 1.f
		};

		unsigned short indices[] = { 0, 1, 2, 1, 3, 2 };

		LAG_GRAPHICS_EXCEPTION(glGenVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_EXCEPTION(glGenBuffers(1, &m_VBO));
		LAG_GRAPHICS_EXCEPTION(glGenBuffers(1, &m_EBO));

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));

		//Setup the VBO
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//Setup the EBO
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

		//Create the frame buffer
		LAG_GRAPHICS_EXCEPTION(glGenFramebuffers(1, &m_FrameBuffer));
		LAG_GRAPHICS_EXCEPTION(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

		//Create a render buffer object for the depth stencil
		LAG_GRAPHICS_EXCEPTION(glGenRenderbuffers(1, &m_DepthStencilBuffer));
		LAG_GRAPHICS_EXCEPTION(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilBuffer));
		LAG_GRAPHICS_EXCEPTION(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GetWindow()->GetWidth(), GetWindow()->GetHeight()));
		LAG_GRAPHICS_EXCEPTION(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilBuffer));

		LAG_GRAPHICS_EXCEPTION(glGenTextures(1, &m_ColorBuffer));
		LAG_GRAPHICS_EXCEPTION(glBindTexture(GL_TEXTURE_2D, m_ColorBuffer));
		LAG_GRAPHICS_EXCEPTION(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetWindow()->GetWidth(), GetWindow()->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer, 0);

		bool bufferStatusSucceeded = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		bool shaderSucceeded = GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/FrameBuffer"));

		if (!(bufferStatusSucceeded && shaderSucceeded))
			Logger::Critical("Failed to create frame buffer.");
		else glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return (bufferStatusSucceeded && shaderSucceeded);
	}

	bool FrameBuffer::Shutdown()
	{
		glDeleteTextures(1, &m_ColorBuffer);
		glDeleteRenderbuffers(1, &m_DepthStencilBuffer);
		glDeleteFramebuffers(1, &m_FrameBuffer);

		glDeleteBuffers(1, &m_VBO);
		glDeleteVertexArrays(1, &m_VAO);
		return false;
	}

	void FrameBuffer::FrameStart(bool showWireframe)
	{
		LAG_GRAPHICS_EXCEPTION(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

		if (showWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_DEPTH_TEST);
		LAG_GRAPHICS_EXCEPTION(glClearColor(0.2f, 0.2f, 0.6f, 1.0f));
		LAG_GRAPHICS_EXCEPTION(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void FrameBuffer::FrameEnd()
	{
		//Second render pass using default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glClearColor(0.7f, 0.f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Bind VAO and Shader
		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));
		Shader* shader = GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/FrameBuffer"));
		shader->Bind();

		//Assign post-processing values
		shader->SetFloat("a_InversionAmount", m_PostProcessingProperties.m_InversionAmount);
		shader->SetFloat("a_GrayScaleAmount", m_PostProcessingProperties.m_GrayScaleAmount);

		LAG_GRAPHICS_EXCEPTION(glBindTexture(GL_TEXTURE_2D, m_ColorBuffer));
		LAG_GRAPHICS_EXCEPTION(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
	}

	void FrameBuffer::Resize()
	{
		//Resize frame buffer
		LAG_GRAPHICS_EXCEPTION(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilBuffer));
		LAG_GRAPHICS_EXCEPTION(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GetWindow()->GetWidth(), GetWindow()->GetHeight()));

		//Resize texture
		LAG_GRAPHICS_EXCEPTION(glBindTexture(GL_TEXTURE_2D, m_ColorBuffer));
		LAG_GRAPHICS_EXCEPTION(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetWindow()->GetWidth(), GetWindow()->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	}

	void* FrameBuffer::GetEditorHandle()
	{
		return (void*)(intptr_t)m_ColorBuffer;
	}
}
