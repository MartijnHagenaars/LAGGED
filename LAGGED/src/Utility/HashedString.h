#pragma once
#include <string>
#include <iostream>

namespace LAG
{
	// What is this class supposed to be doing?
	// I know it hashes the string but why?
	// Are you making some form of String ID?
	class HashedString
	{
	public: 
		HashedString() : 
			m_HashValue(0), m_String("") 
		{}

		explicit HashedString(const std::string& str) :
			m_HashValue(0),m_String(str)
		{
			m_HashValue = Hash(str);
		}

		HashedString(const HashedString& other) :
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
		size_t Hash(const std::string& str)
		{
			std::hash<std::string> hasher;
			return hasher(str);
		}

		std::string m_String;
		size_t m_HashValue;
	};
}