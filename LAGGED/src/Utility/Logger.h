#pragma once
#include <string>

namespace LAG
{
	enum class LoggerSeverity
	{
		Info,
		Warning,
		Error,
		Critical
	};

	class Logger
	{
	public:
		Logger();
		
		static void Placeholder() {}

		static void LogMessage(LoggerSeverity severity, const std::string& file, int line, const std::string& message);
	};

#define INFO(x) Logger::Placeholder()
#define WARNING(x) Logger::Placeholder()
#define ERROR(x) Logger::Placeholder()
#define CRITICAL(x) Logger::Placeholder()

}

