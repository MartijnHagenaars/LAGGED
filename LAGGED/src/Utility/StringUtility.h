#pragma once
#include <string>

namespace LAG
{
	namespace Utility
	{
        //Functions for converting variables into strings

        template<typename T>
        inline std::string ConvertToString(T variable)
        {
            return std::string("[UNDEFINED VARIABLE TYPE]");
        }

        // Specialization for bool
        template<>
        inline std::string ConvertToString<bool>(bool variable)
        {
            return variable ? "true" : "false";
        }

        // Specialization for string
        template<>
        inline std::string ConvertToString<std::string>(std::string variable)
        {
            return variable;
        }

        // Specialization for int
        template<>
        inline std::string ConvertToString<int>(int variable)
        {
            return std::to_string(variable);
        }

        // Specialization for unsigned int
        template<>
        inline std::string ConvertToString<unsigned int>(unsigned int variable)
        {
            return std::to_string(variable);
        }

        // Specialization for float
        template<>
        inline std::string ConvertToString<float>(float variable)
        {
            return std::to_string(variable);
        }
	}
}