#pragma once
#include <vector>
#include <memory>

#include "BufferLayout.h"

namespace LAG
{
	class VertexBuffer
	{
		friend class ArrayBuffer;
	public: 
		void SetVertexData(const void* data, uint32_t size);
		void SetLayout(const BufferLayout& layout) { m_BufferLayout = layout; }

		void Bind();
		void Unbind();

	private:
		//FIXME: Needs to be moved to a platform-specific structure.
		unsigned int m_VBO = 0;

		const void* m_VertexData = nullptr;
		uint32_t m_VertexDataSize = 0;
		BufferLayout m_BufferLayout;
	};

	class IndexBuffer
	{
		friend class ArrayBuffer;
	public:
		void SetIndexData(const std::vector<uint32_t>& data);

		void Bind();
		void Unbind();

		size_t GetCount() const { return m_IndexData.size(); }

	private:
		//FIXME: Needs to be moved to a platform-specific structure.
		unsigned int m_EBO = 0;

		std::vector<uint32_t> m_IndexData;
	};

	class ArrayBuffer
	{
	public: 
		void Initialize(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer);
		void Shutdown();

		void Render();

	private:
		int ConvertBufferVarTypeToGLType(BufferVariableType type);

		//FIXME: Needs to be moved to a platform-specific structure.
		unsigned int m_VAO = 0;

		bool m_Initialized = false;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	};
}

