#include "Precomp.h"
#include "GL_FrameBuffer.h"

#include "Core/Engine.h"
#include "Core/Resources/Shader.h"
#include "Core/Resources/ResourceManager.h"

#include "GL/glew.h"
#include "Platform/Base/Window/WindowManager.h"
#include "Platform/OpenGL/Renderer/Exceptions/GL_GraphicsExceptionMacros.h"


namespace LAG
{
	FrameBuffer::FrameBuffer()
	{
		if (!Initialize())
			Utility::Logger::Critical("Failed to create frame buffer.");
	}

	FrameBuffer::~FrameBuffer()
	{
		if (!Shutdown())
			Utility::Logger::Critical("Failed to shut down frame buffer.");
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

		LAG_GRAPHICS_EXCEPTION(glGenVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_EXCEPTION(glGenBuffers(1, &m_VBO));

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//Create the frame buffer
		LAG_GRAPHICS_EXCEPTION(glGenFramebuffers(1, &m_FrameBuffer));
		LAG_GRAPHICS_EXCEPTION(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

		//Create a render buffer object for the depth stencil
		LAG_GRAPHICS_EXCEPTION(glGenRenderbuffers(1, &m_DepthStencilBuffer));
		LAG_GRAPHICS_EXCEPTION(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilBuffer));
		LAG_GRAPHICS_EXCEPTION(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GetWindowManager()->GetPrimaryWindow()->GetWidth(), GetWindowManager()->GetPrimaryWindow()->GetHeight()));
		LAG_GRAPHICS_EXCEPTION(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilBuffer));

		LAG_GRAPHICS_EXCEPTION(glGenTextures(1, &m_ColorBuffer));
		LAG_GRAPHICS_EXCEPTION(glBindTexture(GL_TEXTURE_2D, m_ColorBuffer));
		LAG_GRAPHICS_EXCEPTION(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetWindowManager()->GetPrimaryWindow()->GetWidth(), GetWindowManager()->GetPrimaryWindow()->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		LAG_GRAPHICS_EXCEPTION(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer, 0);

		bool bufferStatusSucceeded = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		bool shaderSucceeded = GetResourceManager()->AddResource<Shader>(Utility::String("res/Shaders/OpenGL/FrameBuffer"));

		if (!(bufferStatusSucceeded && shaderSucceeded))
			Utility::Logger::Critical("Failed to create frame buffer.");
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

	void FrameBuffer::FrameStart()
	{
		LAG_GRAPHICS_EXCEPTION(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

		glEnable(GL_DEPTH_TEST);
		LAG_GRAPHICS_EXCEPTION(glClearColor(0.2f, 0.2f, 0.6f, 1.0f));
		LAG_GRAPHICS_EXCEPTION(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	}

	void FrameBuffer::FrameEnd()
	{
		////Might not be necessary
		//LAG_GRAPHICS_EXCEPTION(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		//glDisable(GL_DEPTH_TEST);

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));
		GetResourceManager()->GetResource<Shader>(Utility::String("res/Shaders/OpenGL/FrameBuffer"))->Bind();
		LAG_GRAPHICS_EXCEPTION(glBindTexture(GL_TEXTURE_2D, m_ColorBuffer));
		LAG_GRAPHICS_EXCEPTION(glDrawArrays(GL_TRIANGLES, 0, 6));
	}
}
