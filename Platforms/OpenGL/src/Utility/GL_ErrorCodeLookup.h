#pragma once
#include <string>
#include <unordered_map>
#include <GL/glew.h>

#define ERROR_ENTRY(ERROR) {ERROR, #ERROR}

namespace LAG
{
	static const std::unordered_map<int, std::string> ERROR_CODE_LOOKUP_TABLE
	{
		ERROR_ENTRY(GL_INVALID_ENUM),
		ERROR_ENTRY(GL_INVALID_VALUE),
		ERROR_ENTRY(GL_INVALID_OPERATION),
		ERROR_ENTRY(GL_STACK_OVERFLOW),
		ERROR_ENTRY(GL_STACK_UNDERFLOW),
		ERROR_ENTRY(GL_OUT_OF_MEMORY),
		ERROR_ENTRY(GL_INVALID_FRAMEBUFFER_OPERATION),
		ERROR_ENTRY(GL_CONTEXT_LOST),
		ERROR_ENTRY(GL_NO_ERROR)
	};

	static const std::unordered_map<int, std::string> ERROR_TYPE_LOOKUP_TABLE
	{
		ERROR_ENTRY(GL_DEBUG_TYPE_ERROR),
		ERROR_ENTRY(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR),
		ERROR_ENTRY(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR),
		ERROR_ENTRY(GL_DEBUG_TYPE_PORTABILITY),
		ERROR_ENTRY(GL_DEBUG_TYPE_PERFORMANCE),
		ERROR_ENTRY(GL_DEBUG_TYPE_MARKER),
		ERROR_ENTRY(GL_DEBUG_TYPE_PUSH_GROUP),
		ERROR_ENTRY(GL_DEBUG_TYPE_POP_GROUP),
		ERROR_ENTRY(GL_DEBUG_TYPE_OTHER)
	};

	static const std::unordered_map<int, std::string> ERROR_SEVERITY_LOOKUP_TABLE
	{
		ERROR_ENTRY(GL_DEBUG_SEVERITY_HIGH),
		ERROR_ENTRY(GL_DEBUG_SEVERITY_MEDIUM),
		ERROR_ENTRY(GL_DEBUG_SEVERITY_LOW),
		ERROR_ENTRY(GL_DEBUG_SEVERITY_NOTIFICATION)
	};

	static std::string ConvertErrorToString(int errorCode)
	{
		if (errorCode == GL_NO_ERROR)
			return "NO ERROR";

		auto errorIt = ERROR_CODE_LOOKUP_TABLE.find(errorCode);
		if (errorIt == ERROR_CODE_LOOKUP_TABLE.end())
			return "UNDEFINED ERROR";
		else return errorIt->second;
	}

	static std::string ConvertErrorTypeToString(int errorCode)
	{
		if (errorCode == GL_NO_ERROR)
			return "NO ERROR";

		auto errorIt = ERROR_TYPE_LOOKUP_TABLE.find(errorCode);
		if (errorIt == ERROR_TYPE_LOOKUP_TABLE.end())
			return "UNDEFINED TYPE";
		else return errorIt->second;
	}

	static std::string ConvertErrorSeverityToString(int errorCode)
	{
		if (errorCode == GL_NO_ERROR)
			return "NO ERROR";

		auto errorIt = ERROR_SEVERITY_LOOKUP_TABLE.find(errorCode);
		if (errorIt == ERROR_SEVERITY_LOOKUP_TABLE.end())
			return "UNDEFINED SEVERITY";
		else return errorIt->second;
	}
}
