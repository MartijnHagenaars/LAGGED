#pragma once
#include <vector>
#include <string>
#include "BufferLayout.h"

namespace LAG
{
	class IndexBuffer;
	class VertexBuffer;
	class VertexBufferBase
	{
	public:
		VertexBufferBase() = default;
		~VertexBufferBase() = default;

		virtual void SetVertexData(const void* data, uint32_t size) = 0;
		virtual void SetLayout(const BufferLayout& layout) { m_BufferLayout = layout; }

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

	protected:
		const void* m_VertexData = nullptr;
		uint32_t m_VertexDataSize = 0;
		BufferLayout m_BufferLayout;
	};

	class IndexBufferBase
	{
	public:
		IndexBufferBase() = default;
		virtual ~IndexBufferBase() = default;

		virtual void SetIndexData(const std::vector<uint32_t>& data) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		size_t GetCount() const { return m_IndexData.size(); }

	protected:
		std::vector<uint32_t> m_IndexData;
	};

	//TODO: Maybe move this buffer to another file? This one is more important
	class ArrayBufferBase
	{
	public: 
		ArrayBufferBase() = default;
		~ArrayBufferBase() = default;

		virtual void Initialize(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer) = 0;
		virtual void Shutdown() = 0;

		virtual void Render() = 0;

		const VertexBuffer* GetVertexBuffer() const { return m_VertexBuffer; }
		const IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; }

	protected:
		bool m_Initialized = false;
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
	};
}


//Include the correct model header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_Buffers.h"
#elif PLATFORM_DIRECTX

#endif