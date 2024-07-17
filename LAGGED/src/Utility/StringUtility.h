#pragma once
#include <string>

namespace LAG
{
	namespace Utility
	{
		//Functions for converting variables into strings

		template<typename T>
		typename std::enable_if<std::is_arithmetic<T>::value, std::string>::type
			inline ConvertToString(T variable)
		{
			return std::to_string(variable);
		}

		// Primary template for non-arithmetic types
		template<typename T>
		typename std::enable_if<!std::is_arithmetic<T>::value, std::string>::type
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
	}
}