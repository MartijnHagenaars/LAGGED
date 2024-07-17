#pragma once
#include <string>

namespace LAG
{
	namespace Utility
	{
        //Functions for converting variables into strings

        //Convert any number into a string
        template<typename T, std::enable_if_t <std::is_arithmetic<T>::value>> std::string ConvertToString(T variable)
        {
            return std::to_string(variable);
        }

        //Undefined variable type
        template<typename T> std::string ConvertToString(T variable)
        {
            return std::string("[UNDEFINED VARIABLE TYPE]");
        }

        //Convert string into string
        template<>
        inline std::string ConvertToString(std::string variable)
        {
            return variable;
        }

        //Convert const char* into string
        template<>
        inline std::string ConvertToString(const char* variable)
        {
            return std::string(variable);
        }

        //Convert Boolean into string
        template<>
        inline std::string ConvertToString(bool variable)
        {
            return (variable) ? "true" : "false";
        }
	}
}