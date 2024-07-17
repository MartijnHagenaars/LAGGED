#include "Logger.h"

#include "Core/Engine.h"
#include "Platform/Base/PlatformIncludes.h"

namespace LAG
{
	std::string Logger::GenerateMessagePrefix(LoggerSeverity severity, const std::string& file, int line)
	{
		return std::string(GetTimePrefix() + GetSeverityPrefix(severity) + GetFileDataPrefix(file, line) + ": ");
	}

	std::string Logger::GetTimePrefix()
	{
		unsigned int elapsedSeconds = GetEngine().GetElapsedTimeSinceInit() / 1000;

		unsigned int seconds = elapsedSeconds % 60;
		unsigned int minutes = elapsedSeconds / 60;
		if (minutes > 0)
			return std::string("[" + std::to_string(minutes) + "m " + std::to_string(seconds) + "s]");
		else
			return std::string("[" + std::to_string(seconds) + "s]");
	}

	std::string Logger::GetSeverityPrefix(LoggerSeverity severity)
	{
		switch (severity)
		{
		case LoggerSeverity::Info:
			return "[Info]";
		case LoggerSeverity::Warning:
			return "[Warning]";
		case LoggerSeverity::Error:
			return "[Error]";
		case LoggerSeverity::Critical:
			return "[Critical]";
		default:
			return "[Unknown]";
		}
	}

	std::string Logger::GetFileDataPrefix(const std::string& file, int line)
	{
		return std::string("[" + file.substr(file.find_last_of('\\') + 1, file.length()) + "][" + std::to_string(line) + "]");
	}

	void Logger::SetSeverityColor(LoggerSeverity severity)
	{
		HANDLE winHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (severity)
		{
		case LoggerSeverity::Info:
			SetConsoleTextAttribute(winHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			return;
		case LoggerSeverity::Warning:
			SetConsoleTextAttribute(winHandle, FOREGROUND_RED | FOREGROUND_GREEN);
			return;
		case LoggerSeverity::Error:
			SetConsoleTextAttribute(winHandle, FOREGROUND_RED);
			return;
		case LoggerSeverity::Critical:
			SetConsoleTextAttribute(winHandle, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			return;
		default:
			SetConsoleTextAttribute(winHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			return;
		}
	}
}
