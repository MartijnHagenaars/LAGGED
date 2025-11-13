#pragma once
#include <string>

namespace LAG
{
	namespace Utility
	{
		template<typename T>
		constexpr std::string_view GetCleanTypeName()
		{
#ifdef _MSC_VER
			std::string_view funcStr = __FUNCSIG__;
			
			std::string_view firstSubStr = "GetCleanTypeName";
			size_t first = funcStr.find(firstSubStr) + firstSubStr.length() + 1;
			size_t last = funcStr.find_last_of('>') - first;
			std::string_view cleanStr = funcStr.substr(first, last);

			return cleanStr;
#else
			CRITICAL("No currently available implementation for non-MSVC compiler.");
			return std::string_view("");
#endif
		}


		/////////////////////////////////////////////////////
		// Functions for converting variables into strings //
		/////////////////////////////////////////////////////

		// Specialization for arithmetic types
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

		// Specialization for std::string_view
		template<>
		inline std::string ConvertToString<std::string_view>(std::string_view variable)
		{
			return std::string(variable);
		}

		// Specialization for const char*
		template<>
		inline std::string ConvertToString<const char*>(const char* variable)
		{
			return variable;
		}
	}
}