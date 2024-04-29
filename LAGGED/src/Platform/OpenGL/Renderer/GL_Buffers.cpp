#include "GL_Buffers.h"

#include "GL/glew.h"
#include "Platform/OpenGL/Renderer/GL_ErrorChecks.h"

namespace LAG
{
	//Vertex buffer

	void VertexBuffer::SetVertexData(const void* data, uint32_t size)
	{
		m_VertexData = data;
		m_VertexDataSize = size;
		//LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		//LAG_GRAPHICS_CHECK(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	void VertexBuffer::Bind()
	{
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	}

	void VertexBuffer::Unbind()
	{
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	}


	//Index buffer

	void IndexBuffer::SetIndexData(const std::vector<uint32_t>& data)
	{
		if (data.size() > 0)
		{
			m_IndexData = data;

			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			//LAG_GRAPHICS_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint32_t), &data.data()[0], GL_STATIC_DRAW));
		}
		else Logger::Warning("Index data does not contain any data.");
	}

	void IndexBuffer::Bind()
	{
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
	}

	void IndexBuffer::Unbind()
	{
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
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

	void ArrayBuffer::Initialize(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer)
	{
		//if (vertexBuffer.m_VertexDataSize <= 0 || indexBuffer.m_IndexData.empty())
		//{
		//	Logger::Critical("Vertex/index buffer is empty.");
		//	return;
		//}

		m_VertexBuffer = new VertexBuffer(vertexBuffer);
		m_IndexBuffer = new IndexBuffer(indexBuffer);
		

		LAG_GRAPHICS_CHECK(glBindVertexArray(m_VAO));

		//Setup vertex buffer
		LAG_GRAPHICS_CHECK(glCreateBuffers(1, &m_VertexBuffer->m_VBO));
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer->m_VBO));
		LAG_GRAPHICS_CHECK(glBufferData(GL_ARRAY_BUFFER, m_VertexBuffer->m_VertexDataSize, m_VertexBuffer->m_VertexData, GL_STATIC_DRAW));

		//Setup index buffer
		LAG_GRAPHICS_CHECK(glCreateBuffers(1, &m_IndexBuffer->m_EBO));
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer->m_EBO));
		LAG_GRAPHICS_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer->m_IndexData.size() * sizeof(uint32_t), &m_IndexBuffer->m_IndexData.data()[0], GL_STATIC_DRAW));

		//Setup buffer layout
		for (int i = 0; i < m_VertexBuffer->m_BufferLayout.GetBufferLayout().size(); i++)
		{
			BufferLayoutElement bufferElement = m_VertexBuffer->m_BufferLayout.GetBufferLayout()[i];
			if (bufferElement.type >= BufferVariableType::Float1 && bufferElement.type <= BufferVariableType::Float4)
			{
				uint32_t size = GetBufferVariableTypeCount(bufferElement.type);
				int stride = m_VertexBuffer->m_BufferLayout.GetStride();
				int offset = bufferElement.offset * GetBufferVariableTypeSize(bufferElement.type);
				(glVertexAttribPointer(i, size, ConvertBufferVarTypeToGLType(bufferElement.type), bufferElement.isNormalized, stride, (void*)(offset)));
				LAG_GRAPHICS_CHECK(glEnableVertexAttribArray(i));
				continue;
			}
			else if (bufferElement.type >= BufferVariableType::Int1 && bufferElement.type <= BufferVariableType::Bool)
			{
				LAG_GRAPHICS_CHECK(glVertexAttribIPointer(i, GetBufferVariableTypeCount(bufferElement.type), ConvertBufferVarTypeToGLType(bufferElement.type), m_VertexBuffer->m_BufferLayout.GetStride(), (void*)bufferElement.offset));
				LAG_GRAPHICS_CHECK(glEnableVertexAttribArray(i));
				continue;
			}
			else Logger::Critical("Undefined buffer element type.");
		}

		m_Initialized = true;
	}

	void ArrayBuffer::Shutdown()
	{
		LAG_GRAPHICS_CHECK(glDeleteBuffers(1, &m_VertexBuffer->m_VBO));
		LAG_GRAPHICS_CHECK(glDeleteBuffers(1, &m_IndexBuffer->m_EBO));
		LAG_GRAPHICS_CHECK(glDeleteVertexArrays(1, &m_VAO));
	}

	void ArrayBuffer::Render()
	{
		//Check if buffer is initialized
		if (!m_Initialized)
		{
			Logger::Error("Cannot rendert buffer: not initialized.");
			return;
		}

		LAG_GRAPHICS_CHECK(glBindVertexArray(m_VAO));
		LAG_GRAPHICS_CHECK(glDrawElements(GL_TRIANGLES, static_cast<int>(m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, 0));
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
