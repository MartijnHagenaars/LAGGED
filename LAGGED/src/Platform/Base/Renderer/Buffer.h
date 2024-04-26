#pragma once
#include <vector>
#include <string>

namespace LAG
{

	class VertexBufferBase
	{
	public:
		VertexBufferBase() = default;
		virtual ~VertexBufferBase() = default;

		virtual void SetVertexData(const void* data, size_t size) = 0;
		virtual void SetLayout();

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Render();
	};

	class IndexBufferBase
	{
	public:
		IndexBufferBase() = default;
		virtual ~IndexBufferBase() = default;

		virtual void SetIndexData(const std::vector<unsigned int>& data);
	};
}