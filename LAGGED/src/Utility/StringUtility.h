#pragma once
#include <string>
#include <type_traits>

namespace LAG
{
	namespace Utility
	{
        //Functions for converting variables into strings
		template<typename T> 
        std::enable_if_t<std::is_arithmetic<T>>
        ConvertToString(T variable)
		{
			return std::to_string(variable);
		}

        template<>
        inline std::string ConvertToString(const std::string& variable)
        {
            return variable;
        }

        template<>
        inline std::string ConvertToString(const char* variable)
        {
            std::string outputStr = variable;
            return outputStr;
        }

        template<>
        inline std::string ConvertToString(bool variable)
        {
            return (variable) ? "true" : "false";
        }
	}
}