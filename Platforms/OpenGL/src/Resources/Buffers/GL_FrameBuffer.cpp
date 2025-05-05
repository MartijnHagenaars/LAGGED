#include "Platform/Resources/Buffers/FrameBuffer.h"

#include <GL/glew.h>

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"

#include "Platform/Window.h"
#include "Platform/Resources/Shader.h"

#include "Utility/Logger.h"
#include "Utility/GL_ErrorChecks.h"

namespace LAG
{
	struct GlData
	{
		unsigned int VBO = 0;
		unsigned int EBO = 0;
		unsigned int VAO = 0;

		unsigned int frameBuffer = 0;
		unsigned int colorBuffer = 0;
		unsigned int depthStencilBuffer = 0;
	};

	FrameBuffer::FrameBuffer()
	{
		if (!Initialize())
			CRITICAL("Failed to create frame buffer.");
	}

	FrameBuffer::~FrameBuffer()
	{
		if (!Shutdown())
			CRITICAL("Failed to shut down frame buffer.");
	}

	bool FrameBuffer::Initialize()
	{
		if (m_DataPtr == nullptr)
			m_DataPtr = new GlData();

		GlData* pData = static_cast<GlData*>(m_DataPtr);

		//First, create the quad for displaying the frame buffer
		float vertices[] =
		{
			-1.f, -1.f,	0.f, 0.f,
			 1.f, -1.f,	1.f, 0.f,
			-1.f,  1.f,	0.f, 1.f,
			 1.f,  1.f,	1.f, 1.f
		};

		unsigned short indices[] = { 0, 1, 2, 1, 3, 2 };

		LAG_GRAPHICS_CHECK(glGenVertexArrays(1, &pData->VAO));
		LAG_GRAPHICS_CHECK(glGenBuffers(1, &pData->VBO));
		LAG_GRAPHICS_CHECK(glGenBuffers(1, &pData->EBO));

		LAG_GRAPHICS_CHECK(glBindVertexArray(pData->VAO));

		//Setup the VBO
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, pData->VBO));
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//Setup the EBO
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pData->EBO));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

		//Create the frame buffer
		LAG_GRAPHICS_CHECK(glGenFramebuffers(1, &pData->frameBuffer));
		LAG_GRAPHICS_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, pData->frameBuffer));

		//Create a render buffer object for the depth stencil
		LAG_GRAPHICS_CHECK(glGenRenderbuffers(1, &pData->depthStencilBuffer));
		LAG_GRAPHICS_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, pData->depthStencilBuffer));
		LAG_GRAPHICS_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GetSize().x, GetSize().y));
		LAG_GRAPHICS_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pData->depthStencilBuffer));

		LAG_GRAPHICS_CHECK(glGenTextures(1, &pData->colorBuffer));
		LAG_GRAPHICS_CHECK(glBindTexture(GL_TEXTURE_2D, pData->colorBuffer));
		LAG_GRAPHICS_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetSize().x, GetSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		LAG_GRAPHICS_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pData->colorBuffer, 0);

		bool bufferStatusSucceeded = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		bool shaderSucceeded = GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/FrameBuffer")) != nullptr;

		if (!(bufferStatusSucceeded && shaderSucceeded))
			CRITICAL("Failed to create frame buffer.");
		else glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return (bufferStatusSucceeded && shaderSucceeded);
	}

	bool FrameBuffer::Shutdown()
	{
		GlData* pData = static_cast<GlData*>(m_DataPtr);

		glDeleteTextures(1, &pData->colorBuffer);
		glDeleteRenderbuffers(1, &pData->depthStencilBuffer);
		glDeleteFramebuffers(1, &pData->frameBuffer);

		glDeleteBuffers(1, &pData->VBO);
		glDeleteVertexArrays(1, &pData->VAO);

		if (pData != nullptr)
			delete pData;
		pData = nullptr;

		return true;
	}

	void FrameBuffer::FrameStart(bool showWireframe)
	{
		GlData* pData = static_cast<GlData*>(m_DataPtr);

		LAG_GRAPHICS_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, pData->frameBuffer));

		if (showWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_DEPTH_TEST);
		LAG_GRAPHICS_CHECK(glClearColor(0.2f, 0.2f, 0.6f, 1.0f));
		LAG_GRAPHICS_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void FrameBuffer::FrameEnd()
	{
		GlData* pData = static_cast<GlData*>(m_DataPtr);

		//Second render pass using default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glClearColor(0.7f, 0.f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//FIXME: TOOLS MANAGER CODE NEEDS TO BE MOVED SOMEWHERE ELSE.
		////We can return early if the viewport tool is open (as we'll be rendering to the viewport tool instead).
		//if (GetToolsManager()->IsToolOpen("CamView"))
		//	return;

		//Bind VAO and Shader
		LAG_GRAPHICS_CHECK(glBindVertexArray(pData->VAO));
		Shader* shader = GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/FrameBuffer"));
		shader->Bind();

		//Assign post-processing values
		shader->SetFloat("a_InversionAmount", m_PostProcessingProperties.m_InversionAmount);
		shader->SetFloat("a_GrayScaleAmount", m_PostProcessingProperties.m_GrayScaleAmount);

		LAG_GRAPHICS_CHECK(glBindTexture(GL_TEXTURE_2D, pData->colorBuffer));
		LAG_GRAPHICS_CHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
	}

	void FrameBuffer::Resize(const glm::uvec2& size)
	{
		GlData* pData = static_cast<GlData*>(m_DataPtr);

		m_FrameBufferSize = size;

		//Resize frame buffer
		LAG_GRAPHICS_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, pData->depthStencilBuffer));
		LAG_GRAPHICS_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GetSize().x, GetSize().y));

		//Resize texture
		LAG_GRAPHICS_CHECK(glBindTexture(GL_TEXTURE_2D, pData->colorBuffer));
		LAG_GRAPHICS_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetSize().x, GetSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	}

	void* FrameBuffer::GetEditorHandle()
	{
		GlData* pData = static_cast<GlData*>(m_DataPtr);
		return (void*)(intptr_t)pData->colorBuffer;
	}

	glm::uvec2 FrameBuffer::GetSize() const
	{
		if (m_UseWindowSize)
			return glm::uvec2(GetWindow()->GetWidth(), GetWindow()->GetHeight());
		else
			return m_FrameBufferSize;
	}
}
