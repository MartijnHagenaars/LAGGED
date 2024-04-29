#pragma once
#include "spdlog/spdlog.h"

namespace LAG::Logger
{
	void Initialize();
	void Shutdown();

	template<typename Msg, typename... Args>
	void Info(const Msg& fmt, const Args&... args)
	{
		spdlog::info(fmt, args...);
	}

	template<typename Msg, typename... Args>
	void Warning(const Msg& fmt, const Args&... args)
	{
		spdlog::warn(fmt, args...);
	}

	template<typename Msg, typename... Args>
	void Error(const Msg& fmt, const Args&... args)
	{
		spdlog::error(fmt, args...);
	}

	template<typename Msg, typename... Args>
	void Critical(const Msg& fmt, const Args&... args)
	{
		spdlog::critical(fmt, args...);
		__debugbreak;
	}
}

