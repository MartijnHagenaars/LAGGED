#pragma once
#include "Core/Defines.h"
#include "StringUtility.h"

namespace LAG
{
	// 64 bit implementation of the FNV-1A hashing algorithm
	constexpr Hash64 StringToHash64(std::string_view str, const uint64_t prime = 1099511628211)
	{
		const Hash64 offset = 14695981039346656037;
		Hash64 hash = offset;

		for (char c : str) 
			hash = (hash ^ static_cast<unsigned char>(c)) * prime;

		return hash;
	}

	template<typename T>
	constexpr Hash64 GetTypeHash64()
	{
		return StringToHash64(Utility::GetCleanTypeName<T>());
	}

	// TODO / FIXME: This class is currently NOT using constexpr's and needs to be fixed ASAP!
	class StringHash64
	{
	public:
		StringHash64() :
			m_Value(0), m_String("")
		{
		}

		explicit StringHash64(const std::string& str) :
			m_Value(0), m_String(str)
		{
			// FIXME: Needs to be moved to initializer list
			m_Value = StringToHash64(str);
		}

		StringHash64(const StringHash64& other) :
			m_Value(other.m_Value), m_String(other.m_String)
		{
		}

		Hash64 Value() const
		{
			return m_Value;
		}

		std::string String() const
		{
			return std::string(m_String);
		}

	private:
		Hash64 m_Value;
		std::string m_String;
	};
}