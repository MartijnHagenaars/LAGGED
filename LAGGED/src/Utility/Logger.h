#pragma once

namespace LAG::Logger
{
	//void Initialize() {};
	//void Shutdown() {};

	template<typename Msg, typename... Args>
	void Info(const Msg& fmt, const Args&... args)
	{
		//TODO: Implement
	}

	template<typename Msg, typename... Args>
	void Warning(const Msg& fmt, const Args&... args)
	{
		//TODO: Implement
	}

	template<typename Msg, typename... Args>
	void Error(const Msg& fmt, const Args&... args)
	{
		//TODO: Implement
	}

	template<typename Msg, typename... Args>
	void Critical(const Msg& fmt, const Args&... args)
	{
		//TODO: Implement
		__debugbreak();
	}
}

