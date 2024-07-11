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
		Logger() = default;

		static void LogMessage(LoggerSeverity severity, const std::string& file, int line, const std::string& message);
	};


#undef ERROR

#define INFO(x) LAG::Logger::LogMessage(LAG::LoggerSeverity::Info, __FILE__, __LINE__, "This is hardcoded info!")
#define WARNING(x) LAG::Logger::LogMessage(LAG::LoggerSeverity::Warning, __FILE__, __LINE__, "This is a hardcoded warning!!")
#define ERROR(x) LAG::Logger::LogMessage(LAG::LoggerSeverity::Error, __FILE__, __LINE__, "This is a hardcoded error message!!!")
#define CRITICAL(x) do { LAG::Logger::LogMessage(LAG::LoggerSeverity::Critical, __FILE__, __LINE__, "This is a hardcoded critical message! Oh noes!!"); __debugbreak(); } while(false)

}

