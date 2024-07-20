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

		static void SetSeverityColor(LoggerSeverity severity);

		template <class ... Arg>
		static inline std::string ApplyArgumentsToFormat(std::string message, Arg & ...args);
	};



#undef ERROR
#define INFO(msg, ...) LAG::Logger::LogMessage(LAG::LoggerSeverity::Info, __FILE__, __LINE__, msg, __VA_ARGS__)
#define WARNING(msg, ...) LAG::Logger::LogMessage(LAG::LoggerSeverity::Warning, __FILE__, __LINE__, msg, __VA_ARGS__)
#define ERROR(msg, ...) LAG::Logger::LogMessage(LAG::LoggerSeverity::Error, __FILE__, __LINE__, msg, __VA_ARGS__)
#define CRITICAL(msg, ...) do { LAG::Logger::LogMessage(LAG::LoggerSeverity::Critical, __FILE__, __LINE__, msg, __VA_ARGS__); __debugbreak(); } while(false)

	template<typename ...Args>
	inline void Logger::LogMessage(LoggerSeverity severity, const std::string& file, int line, const std::string& messageFormat, const Args & ...arguments)
	{
		std::string message = ApplyArgumentsToFormat(messageFormat, arguments...);

		//Set text color and print message
		SetSeverityColor(severity);
		printf("%s%s\n", GenerateMessagePrefix(severity, file, line).c_str(), message.c_str());

		//Reset the color
		SetSeverityColor(LoggerSeverity::Info);
	}

	template<class ...Arg>
	inline std::string Logger::ApplyArgumentsToFormat(std::string message, Arg & ...args)
	{
		if (sizeof...(args) > 0)
		{
			int count = 0;
			([&args, &message, &count]
				{
					const auto& unorderedParamPos = message.find("{}");
					if (unorderedParamPos != std::string::npos)
					{
						message.erase(message.begin() + unorderedParamPos, message.begin() + unorderedParamPos + 2);
						message.insert(unorderedParamPos, Utility::ConvertToString(args));
					}

					size_t orderedParamPos = std::string::npos;
					while ((orderedParamPos = message.find("{" + std::to_string(count) + "}")) != std::string::npos)
					{
						message.erase(message.begin() + orderedParamPos, message.begin() + orderedParamPos + 3);
						message.insert(orderedParamPos, Utility::ConvertToString(args));
					}

					++count;
				} (), ...);
		}

		return message;
	}
}

