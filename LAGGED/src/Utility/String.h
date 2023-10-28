#pragma once
#include <string>
#include <iostream>

namespace LAG::Utility
{
	class String
	{
	public: 
		String() = delete;
		explicit constexpr String(const char* s) :
			m_String(s), m_HashValue(0)
		{
			m_HashValue = Hash(s);
		}

		String(const String& other) : 
			m_String(other.m_String), m_HashValue(other.m_HashValue)
		{}

		size_t GetValue() const
		{
			return m_HashValue;
		}

		std::string GetString() const
		{
			return std::string(m_String);
		}

	private:
		constexpr size_t Hash(const char* s)
		{
			size_t stringLength = std::char_traits<char>::length(s);
			size_t sum = 0; 
			for (int i = 0; i < stringLength; i++) {
				sum += (s[i] * (int)pow(PRIME_VALUE, i)) % ARR_SIZE;
			}

			return sum;
		}

		const char* m_String;
		size_t m_HashValue;
		const int PRIME_VALUE = 31;
		const int ARR_SIZE = 3001;
	};
}