#include "Logger.h"

#include "Platform/Base/PlatformIncludes.h"
#include "Core/Defines.h"

namespace LAG
{
	void Logger::LogMessage(LoggerSeverity severity, const std::string& file, int line, const std::string& message)
	{
		std::string severityDisplay;
		switch (severity)
		{
		case LoggerSeverity::Info:
			severityDisplay = "Info";
			break;
		case LoggerSeverity::Warning:
			severityDisplay = "Warning";
			break;
		case LoggerSeverity::Error:
			severityDisplay = "Error";
			break;
		case LoggerSeverity::Critical:
			severityDisplay = "Critical";
			break;
		default: 
			severityDisplay = "Unknown";
			break;

		}
	}
}
