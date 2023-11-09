#pragma once
#include <string>
#include <iostream>

namespace LAG::Utility
{
	class String
	{
	public: 
		String() = delete;
		explicit String(const std::string& str) :
			m_HashValue(0),m_String(str)
		{
			m_HashValue = Hash(str);
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
		size_t Hash(const std::string& str, int i = 0)
		{
			std::hash<std::string> hasher;
			return hasher(str);

			//size_t charValue = static_cast<size_t>(str[i]);
			//if (str[i] != '\0')
			//	return (Hash(str.data(), i + 1) * 32) + charValue;
			//else
			//	return 26;
		}

		std::string m_String;
		size_t m_HashValue;
	};
}