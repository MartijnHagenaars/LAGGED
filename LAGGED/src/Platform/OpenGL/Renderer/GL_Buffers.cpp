#include "GL_Buffers.h"

#include "GL/glew.h"
#include "Platform/OpenGL/Renderer/GL_ErrorChecks.h"

namespace LAG
{
	//Vertex buffer

	VertexBuffer::~VertexBuffer()
	{
		//TODO: Remove vertex buffer
	}

	void VertexBuffer::SetVertexData(const void* data, uint32_t size)
	{
	}

	void VertexBuffer::Bind()
	{
	}

	void VertexBuffer::Unbind()
	{
		LAG_GRAPHICS_CHECK(glDeleteBuffers(1, &m_VBO));
	}


	//Index buffer

	IndexBuffer::~IndexBuffer()
	{
		LAG_GRAPHICS_CHECK(glDeleteBuffers(1, &m_EBO));
	}

	void IndexBuffer::SetIndexData(const std::vector<uint32_t>& data)
	{
	}


	//Array buffer

	ArrayBuffer::ArrayBuffer()
	{
		LAG_GRAPHICS_CHECK(glCreateVertexArrays(1, &m_VAO));
	}

	ArrayBuffer::~ArrayBuffer()
	{
		LAG_GRAPHICS_CHECK(glDeleteBuffers(1, &m_VAO));
	}

	//TODO: Passing by reference might be bad since we're moving ownership.... Need to test this.
	void ArrayBuffer::Initialize(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer)
	{
		if (vertexBuffer.m_VertexDataSize <= 0 || indexBuffer.m_IndexData.empty())
		{
			Logger::Critical("Vertex/index buffer is empty.");
			return;
		}

		m_VertexBuffer = std::move(&vertexBuffer);
		m_IndexBuffer = std::move(&indexBuffer);

		LAG_GRAPHICS_CHECK(glBindVertexArray(m_VAO));

		//Setup vertex buffer
		for (int i = 0; i < m_VertexBuffer->m_BufferLayout.GetBufferLayout().size(); i++)
		{
			BufferLayoutElement bufferElement = m_VertexBuffer->m_BufferLayout.GetBufferLayout()[i];
			if (bufferElement.type >= BufferVariableType::Float1 && bufferElement.type <= BufferVariableType::Float4)
			{
				//TODO: Calculate offset
				glVertexAttribPointer(i, GetBufferVariableTypeSize(bufferElement.type), ConvertBufferVarTypeToGLType(bufferElement.type), bufferElement.isNormalized, m_VertexBuffer->m_BufferLayout.GetStride(), (void*)bufferElement.offset);
				glEnableVertexAttribArray(i);
				continue;
			}
			else if (bufferElement.type >= BufferVariableType::Int1 && bufferElement.type <= BufferVariableType::Bool)
			{
				glVertexAttribIPointer(i, GetBufferVariableTypeSize(bufferElement.type), ConvertBufferVarTypeToGLType(bufferElement.type), m_VertexBuffer->m_BufferLayout.GetStride(), (void*)bufferElement.offset);
				glEnableVertexAttribArray(i);
				continue;
			}
			else Logger::Critical("Undefined buffer element type.");
		}

		//Setup index buffer

		glCreateBuffers(1, &m_IndexBuffer->m_EBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_IndexBuffer->m_EBO);
		glBufferData(GL_ARRAY_BUFFER, m_IndexBuffer->m_IndexData.size() * sizeof(uint32_t), m_IndexBuffer->m_IndexData.data(), GL_STATIC_DRAW);

		m_Initialized = true;
	}

	void ArrayBuffer::Render()
	{
		//Check if buffer is initialized
		if (!m_Initialized)
		{
			Logger::Error("Cannot rendert buffer: not initialized.");
			return;
		}

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	int ArrayBuffer::ConvertBufferVarTypeToGLType(BufferVariableType type)
	{
		switch (type)
		{
		case BufferVariableType::Float1: return GL_FLOAT;
		case BufferVariableType::Float2: return GL_FLOAT;
		case BufferVariableType::Float3: return GL_FLOAT;
		case BufferVariableType::Float4: return GL_FLOAT;
		case BufferVariableType::Int1: return GL_INT;
		case BufferVariableType::Int2: return GL_INT;
		case BufferVariableType::Int3: return GL_INT;
		case BufferVariableType::Int4: return GL_INT;
		case BufferVariableType::Mat2: return GL_FLOAT;
		case BufferVariableType::Mat3: return GL_FLOAT;
		case BufferVariableType::Mat4: return GL_FLOAT;
		case BufferVariableType::Bool: return GL_BOOL;
		default: return 0;
		}
	}
};
