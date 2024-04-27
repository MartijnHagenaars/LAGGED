#include "GL_Buffers.h"

#include "GL/glew.h"
#include "Platform/OpenGL/Renderer/GL_ErrorChecks.h"

namespace LAG
{
	//Vertex buffer
	
	VertexBuffer::~VertexBuffer()
	{
	}

	void VertexBuffer::SetVertexData(const void* data, uint32_t size)
	{
	}

	void VertexBuffer::Bind()
	{
	}

	void VertexBuffer::Unbind()
	{
	}



	//Array buffer

	ArrayBuffer::ArrayBuffer()
	{
		LAG_GRAPHICS_CHECK(glCreateVertexArrays(1, &m_VAO));
	}

	ArrayBuffer::~ArrayBuffer()
	{
	}

	void ArrayBuffer::Initialize(const VertexBufferBase& vertexBuffer, const IndexBufferBase& indexBuffer)
	{

	}
}
