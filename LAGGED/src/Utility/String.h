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
			m_HashValue(0)
		{
			m_HashValue = Hash(s);
		}

		String(const String& other)
		{
			this->m_HashValue = other.m_HashValue;
		}

		size_t GetValue() const
		{
			return m_HashValue;
		}

	private:
		constexpr size_t Hash(const char* s)
		{
			size_t stringLength = sizeof(s);

			size_t sum = 0; 
			for (int i = 0; i < stringLength; i++) {
				sum += (s[i] * (int)pow(PRIME_CONST, i)) % ARR_SIZE;
			}

			return sum;
		}

		size_t m_HashValue;
		static const int PRIME_CONST = 31;
		static const int ARR_SIZE = 3001;
	};
}