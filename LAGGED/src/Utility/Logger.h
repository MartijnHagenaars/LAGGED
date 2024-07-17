#pragma once
#include <string>
#include "Utility/StringUtility.h"

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

		template <class ... Arg>
		static inline std::string ApplyArgumentsToFormat(std::string message, Arg & ...inputs);
	};



#undef ERROR
#define INFO(msg, ...) LAG::Logger::LogMessage(LAG::LoggerSeverity::Info, __FILE__, __LINE__, msg, __VA_ARGS__)
#define WARNING(msg, ...) LAG::Logger::LogMessage(LAG::LoggerSeverity::Warning, __FILE__, __LINE__, msg, __VA_ARGS__)
#define ERROR(msg, ...) LAG::Logger::LogMessage(LAG::LoggerSeverity::Error, __FILE__, __LINE__, msg, __VA_ARGS__)
#define CRITICAL(msg, ...) do { LAG::Logger::LogMessage(LAG::LoggerSeverity::Critical, __FILE__, __LINE__, msg, __VA_ARGS__); __debugbreak(); } while(false)

	template<typename ...Args>
	inline void Logger::LogMessage(LoggerSeverity severity, const std::string& file, int line, const std::string& messageFormat, const Args & ...arguments)
	{
		std::string newMessage = ApplyArgumentsToFormat(messageFormat, arguments...);

		printf("%s%s\n", GenerateMessagePrefix(severity, file, line).c_str(), newMessage.c_str());
	}

	template<class ...Arg>
	inline std::string Logger::ApplyArgumentsToFormat(std::string message, Arg & ...inputs)
	{
		int count = 0;
		
		([&inputs, &message, &count]
			{
				const auto& unorderedParamPos = message.find("{}");
				if (unorderedParamPos != std::string::npos)
					message.replace(unorderedParamPos, unorderedParamPos + 2, Utility::ConvertToString(inputs));
				
				size_t orderedParamPos = std::string::npos;
				while((orderedParamPos = message.find("{" + std::to_string(count) + "}")) != std::string::npos)
					message.replace(orderedParamPos, orderedParamPos + 3, Utility::ConvertToString(inputs));

				++count;
			} (), ...);

		return message;
	}
}

