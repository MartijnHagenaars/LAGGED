#pragma once
#include <string>

namespace LAG
{
	namespace Utility
	{
		//Functions for converting variables into strings

		//Specialization for arithmetic types
		template<typename T>
		typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, long>::value || std::is_same<T, long long>::value ||
			std::is_same<T, unsigned>::value || std::is_same<T, unsigned long>::value || std::is_same<T, unsigned long long>::value ||
			std::is_same<T, float>::value || std::is_same<T, double>::value || std::is_same<T, long double>::value, std::string>::type
			inline ConvertToString(T variable)
		{
			return std::to_string(variable);
		}

		// Primary template for unknown types
		template<typename T>
		typename std::enable_if<!std::is_same<T, int>::value && !std::is_same<T, long>::value && !std::is_same<T, long long>::value &&
			!std::is_same<T, unsigned>::value && !std::is_same<T, unsigned long>::value && !std::is_same<T, unsigned long long>::value &&
			!std::is_same<T, float>::value && !std::is_same<T, double>::value && !std::is_same<T, long double>::value, std::string>::type
			inline ConvertToString(T variable)
		{
			return std::string("[UNDEFINED VARIABLE TYPE]");
		}

		// Specialization for bool
		template<>
		inline std::string ConvertToString<bool>(bool variable)
		{
			return variable ? "true" : "false";
		}

		// Specialization for std::string
		template<>
		inline std::string ConvertToString<std::string>(std::string variable)
		{
			return variable;
		}

		// Specialization for const char*
		template<>
		inline std::string ConvertToString<const char*>(const char* variable)
		{
			return variable;
		}
	}
}