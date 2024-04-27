#pragma once
#include "Platform/Base/Renderer/Buffer.h"

namespace LAG
{
	class VertexBuffer final : public VertexBufferBase
	{
	public: 
		VertexBuffer() = delete;
		~VertexBuffer();

		void SetVertexData(const void* data, uint32_t size) override;

		void Bind() override;
		void Unbind() override;

	private:
		unsigned int m_VBO = 0;
	};

	class ArrayBuffer final : public ArrayBufferBase
	{
	public: 
		ArrayBuffer();
		~ArrayBuffer();

		void Initialize(const VertexBufferBase& vertexBuffer, const IndexBufferBase& indexBuffer) override;

	private:
		unsigned int m_VAO = 0;
	};
}

