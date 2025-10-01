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

	class StringHash
	{
	public:
		StringHash() :
			m_HashValue(0), m_String("")
		{
		}

		explicit StringHash(const std::string& str) :
			m_HashValue(0), m_String(str)
		{
			m_HashValue = Hash(str);
		}

		StringHash(const StringHash& other) :
			m_String(other.m_String), m_HashValue(other.m_HashValue)
		{
		}

		size_t GetValue() const
		{
			return m_HashValue;
		}

		std::string GetString() const
		{
			return std::string(m_String);
		}

	private:
		size_t Hash(const std::string& str)
		{
			std::hash<std::string> hasher;
			return hasher(str);
		}

		std::string m_String;
		size_t m_HashValue;
	};
}