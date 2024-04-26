#pragma once
#include <string>

namespace LAG
{
	enum class BufferVariableType
	{
		Int1 = 0,
		Int2,
		Int3,
		Int4,
		Float1,
		Float2,
		Float3,
		Float4,
		Mat2,
		Mat3,
		Mat4,
		Bool
	};

	struct BufferLayoutItem
	{
		BufferLayoutItem() = delete;
		BufferLayoutItem(const std::string& name, BufferVariableType type, bool isNormalized) :
			name(name), type(type), isNormalized(isNormalized)
		{}

		std::string name;
		BufferVariableType type;
		bool isNormalized;
	};
}