#pragma once
#include <string>
#include <vector>

namespace LAG
{
	enum class BufferVariableType;
	struct BufferLayoutElement
	{
		//TODO: Remove name; it is useless!
		BufferLayoutElement() = delete;
		BufferLayoutElement(const std::string& name, BufferVariableType type, bool isNormalized) :
			/*name(name), */type(type), isNormalized(isNormalized)
		{}

		//std::string name;
		BufferVariableType type;
		bool isNormalized;
		uint32_t offset;
	};

	static uint32_t GetBufferVariableTypeSize(BufferVariableType type);
	static uint32_t GetBufferVariableTypeCount(BufferVariableType type);

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::vector<BufferLayoutElement>& layout) :
			m_BufferLayout(layout), m_Stride(0)
		{
			for (auto& it : m_BufferLayout)
			{
				it.offset = m_Stride;
				m_Stride += GetBufferVariableTypeSize(it.type) * GetBufferVariableTypeCount(it.type);
			}
		}

		int GetStride() const { return m_Stride; }
		const std::vector<BufferLayoutElement>& GetBufferLayout() const { return m_BufferLayout; }

	private:
		int m_Stride;
		std::vector<BufferLayoutElement> m_BufferLayout;
	};



	enum class BufferVariableType
	{
		Int1, Int2, Int3, Int4, Bool,
		Float1, Float2, Float3, Float4,
		Mat2, Mat3, Mat4,
	};

	inline uint32_t GetBufferVariableTypeSize(BufferVariableType type)
	{
		switch (type)
		{
		case BufferVariableType::Int1:
		case BufferVariableType::Int2:
		case BufferVariableType::Int3:
		case BufferVariableType::Int4:
			return sizeof(int);
		case BufferVariableType::Float1: 
		case BufferVariableType::Float2: 
		case BufferVariableType::Float3: 
		case BufferVariableType::Float4: 
		case BufferVariableType::Mat2: 
		case BufferVariableType::Mat3: 
		case BufferVariableType::Mat4: 
			return sizeof(float);
		case BufferVariableType::Bool: 
			return sizeof(bool);
		default: return 0; //This should never happen.
		}
	}

	inline uint32_t GetBufferVariableTypeCount(BufferVariableType type)
	{
		switch (type)
		{
		case BufferVariableType::Int1: return 1;
		case BufferVariableType::Int2: return 2;
		case BufferVariableType::Int3: return 3;
		case BufferVariableType::Int4: return 4;
		case BufferVariableType::Float1: return 1;
		case BufferVariableType::Float2: return 2;
		case BufferVariableType::Float3: return 3;
		case BufferVariableType::Float4: return 4;
		case BufferVariableType::Mat2: return 2 * 2;
		case BufferVariableType::Mat3: return 3 * 3;
		case BufferVariableType::Mat4: return 3 * 3;
		case BufferVariableType::Bool: return 1;
		default: return 0; //This should never happen.
		}
	}
}