#pragma once
#include "Platform/Base/Renderer/Buffer.h"

namespace LAG
{
	class VertexBuffer final : public VertexBufferBase
	{
		friend class ArrayBuffer;
	public: 
		VertexBuffer() = delete;
		~VertexBuffer();

		void SetVertexData(const void* data, uint32_t size) override;

		void Bind() override;
		void Unbind() override;

	private:
		unsigned int m_VBO = 0;
	};

	class IndexBuffer final : public IndexBufferBase
	{
		friend class ArrayBuffer;
	public:
		IndexBuffer() = delete;
		~IndexBuffer();

		void SetIndexData(const std::vector<uint32_t>& data) override;

	private:
		unsigned int m_EBO = 0;
	};

	class ArrayBuffer final : public ArrayBufferBase
	{
	public: 
		ArrayBuffer();
		~ArrayBuffer();

		void Initialize(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer) override;

	private:
		int ConvertBufferVarTypeToGLType(BufferVariableType type);

		unsigned int m_VAO = 0;
	};
}

