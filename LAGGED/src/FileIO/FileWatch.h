#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <filesystem>

namespace LAG
{
	class FileWatch
	{
	public:
		FileWatch() = default;
		~FileWatch() = delete;

		static void Register(const std::string& filePath, std::function<void(const std::string&)> watchCallback);
		static void Remove(const std::string& filePath);

		static void CheckWatches();

	private:
		struct WatchData
		{
			std::string filePath;
			std::function<void(const std::string&)> callbackFunc;

			std::filesystem::file_time_type lastFileTime;

		};
		static std::vector<WatchData> m_WatchedFiles;
	};
}

