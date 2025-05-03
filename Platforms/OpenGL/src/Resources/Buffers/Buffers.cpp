#include "Platform/Resources/Buffers/Buffers.h"

#include <GL/glew.h>

#include "Utility/ErrorChecks.h"

namespace LAG
{
	// Data structures

	struct GlVertexData
	{
		unsigned int VBO = 0;
	};

	struct GlIndexData
	{
		unsigned int EBO = 0;
	};

	struct GlArrayData
	{
		unsigned int VAO = 0;
	};

	//Vertex buffer

	void VertexBuffer::SetVertexData(const void* data, uint32_t size)
	{
		m_Buffer = data;
		m_VertexDataSize = size;
	}

	void VertexBuffer::Bind()
	{
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, static_cast<GlVertexData*>(m_DataPtr)->VBO));
	}

	void VertexBuffer::Unbind()
	{
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}


	//Index buffer

	void IndexBuffer::SetIndexData(const std::vector<uint32_t>& data)
	{
		if (data.size() > 0)
		{
			m_IndexData = data;
		}
		else WARNING("Index data does not contain any data.");
	}

	void IndexBuffer::Bind()
	{
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GlIndexData*>(m_DataPtr)->EBO));
	}

	void IndexBuffer::Unbind()
	{
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}


	//Array buffer

	void ArrayBuffer::Initialize(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer)
	{
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertexBuffer);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indexBuffer);

		//Setup array buffer
		m_DataPtr = new GlArrayData;
		GlArrayData* arrayData = static_cast<GlArrayData*>(m_DataPtr);

		LAG_GRAPHICS_CHECK(glCreateVertexArrays(1, &arrayData->VAO));
		LAG_GRAPHICS_CHECK(glBindVertexArray(arrayData->VAO));

		//Setup vertex buffer
		m_VertexBuffer->m_DataPtr = new GlVertexData;
		GlVertexData* vertexData = static_cast<GlVertexData*>(m_VertexBuffer->m_DataPtr);

		LAG_GRAPHICS_CHECK(glCreateBuffers(1, &vertexData->VBO));
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertexData->VBO));
		LAG_GRAPHICS_CHECK(glBufferData(GL_ARRAY_BUFFER, m_VertexBuffer->m_VertexDataSize, m_VertexBuffer->m_Buffer, GL_STATIC_DRAW));

		//Setup index buffer
		m_IndexBuffer->m_DataPtr = new GlIndexData;
		GlIndexData* indexData = static_cast<GlIndexData*>(m_IndexBuffer->m_DataPtr);

		LAG_GRAPHICS_CHECK(glCreateBuffers(1, &indexData->EBO));
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexData->EBO));
		LAG_GRAPHICS_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer->m_IndexData.size() * sizeof(uint32_t), &m_IndexBuffer->m_IndexData.data()[0], GL_STATIC_DRAW));

		//Setup buffer layout
		for (int i = 0; i < m_VertexBuffer->m_BufferLayout.GetBufferLayout().size(); i++)
		{
			BufferLayoutElement bufferElement = m_VertexBuffer->m_BufferLayout.GetBufferLayout()[i];
			if (bufferElement.type >= BufferVariableType::Float1 && bufferElement.type <= BufferVariableType::Float4)
			{
				uint32_t size = GetBufferVariableTypeCount(bufferElement.type);
				int stride = m_VertexBuffer->m_BufferLayout.GetStride();
				int offset = bufferElement.offset;
				LAG_GRAPHICS_CHECK(glVertexAttribPointer(i, size, ConvertBufferVarTypeToGLType(bufferElement.type), bufferElement.isNormalized, stride, (void*)(offset)));
				LAG_GRAPHICS_CHECK(glEnableVertexAttribArray(i));
				continue;
			}
			else if (bufferElement.type >= BufferVariableType::Int1 && bufferElement.type <= BufferVariableType::Bool)
			{
				LAG_GRAPHICS_CHECK(glVertexAttribIPointer(i, GetBufferVariableTypeCount(bufferElement.type), ConvertBufferVarTypeToGLType(bufferElement.type), m_VertexBuffer->m_BufferLayout.GetStride(), (void*)bufferElement.offset));
				LAG_GRAPHICS_CHECK(glEnableVertexAttribArray(i));
				continue;
			}
			else CRITICAL("Undefined buffer element type.");
		}

		m_Initialized = true;
	}

	void ArrayBuffer::Shutdown()
	{
		if (m_Initialized)
		{
			GlVertexData* vertexData = static_cast<GlVertexData*>(m_VertexBuffer->m_DataPtr);
			LAG_GRAPHICS_CHECK(glDeleteBuffers(1, &vertexData->VBO));
			delete m_VertexBuffer->m_DataPtr;
			m_VertexBuffer->m_DataPtr = nullptr;

			GlIndexData* indexData = static_cast<GlIndexData*>(m_IndexBuffer->m_DataPtr);
			LAG_GRAPHICS_CHECK(glDeleteBuffers(1, &indexData->EBO));
			delete m_IndexBuffer->m_DataPtr;
			m_IndexBuffer->m_DataPtr = nullptr;

			GlArrayData* arrayData = static_cast<GlArrayData*>(m_DataPtr);
			LAG_GRAPHICS_CHECK(glDeleteVertexArrays(1, &arrayData->VAO));
			delete m_DataPtr;
			m_DataPtr = nullptr;

			m_Initialized = false;
		}
	}

	void ArrayBuffer::Render()
	{
		//Check if buffer is initialized
		if (!m_Initialized)
		{
			CRITICAL("Cannot render buffer: not initialized.");
			return;
		}

		LAG_GRAPHICS_CHECK(glBindVertexArray(static_cast<GlArrayData*>(m_DataPtr)->VAO));
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
