#pragma once
#include <string>
#include <iostream>

namespace LAG::Utility
{
	class String
	{
	public: 
		String() = delete;
		LAG_API explicit constexpr String(const char* s) :
			m_String(s), m_HashValue(0)
		{
			m_HashValue = Hash(s);
		}

		LAG_API String(const String& other) : 
			m_HashValue(other.m_HashValue)
		{}

		LAG_API size_t GetValue() const
		{
			return m_HashValue;
		}

		LAG_API std::string GetString() const
		{
			return std::string(m_String);
		}

	private:
		constexpr size_t Hash(const char* s)
		{
			size_t stringLength = sizeof(s);

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