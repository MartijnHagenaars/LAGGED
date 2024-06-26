#pragma once

namespace LAG::Util
{
	template<typename T>
	constexpr const T& Clamp(const T& value, const T& minValue, const T& maxValue)
	{
		return value < minValue ? minValue : value > maxValue ? maxValue : value;
	}
}
