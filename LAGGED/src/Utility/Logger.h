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

		template<typename... Args>
		static inline void LogMessage(LoggerSeverity severity, const std::string& file, int line, const std::string& messageFormat, const Args&... arguments);

		static std::string GenerateMessagePrefix(LoggerSeverity severity, const std::string& file, int line);

	private:
		static std::string GetTimePrefix();
		static std::string GetSeverityPrefix(LoggerSeverity severity);
		static std::string GetFileDataPrefix(const std::string& file, int line);
	};



#undef ERROR
#define INFO(msg) LAG::Logger::LogMessage(LAG::LoggerSeverity::Info, __FILE__, __LINE__, msg)
#define WARNING(msg) LAG::Logger::LogMessage(LAG::LoggerSeverity::Warning, __FILE__, __LINE__, msg)
#define ERROR(msg) LAG::Logger::LogMessage(LAG::LoggerSeverity::Error, __FILE__, __LINE__, msg)
#define CRITICAL(msg) do { LAG::Logger::LogMessage(LAG::LoggerSeverity::Critical, __FILE__, __LINE__, msg); __debugbreak(); } while(false)



	template<typename ...Args>
	inline void Logger::LogMessage(LoggerSeverity severity, const std::string& file, int line, const std::string& messageFormat, const Args & ...arguments)
	{
		printf("%s%s\n", GenerateMessagePrefix(severity, file, line).c_str(), messageFormat.c_str());
	}

}

