#pragma once
#include <string>
#include <functional>
#include <unordered_map>

#include "Core/FileIO/FileIO.h"
#include <filesystem>

namespace LAG
{
	class FileWatch
	{
	public:
		FileWatch() = default;
		~FileWatch() = delete;

		static void Register(FileIO::Directory directory, const std::string& relativePath, std::function<void(const std::string&)> watchCallback);
		static void Remove(FileIO::Directory directory, const std::string& relativePath);

		static bool IsWatchingFile(FileIO::Directory directory, const std::string& relativePath);

		static void CheckWatches();

		static void EnableFileWatch(bool enable);
		static bool IsEnabled() { return m_IsEnabled; }

	private:
		struct WatchData
		{
			std::string filePath;
			std::function<void(const std::string&)> callbackFunc;

			std::filesystem::file_time_type lastFileTime;
		};

		//TODO: Consider changing this to a map. Need to profile this...
		static inline std::vector<WatchData> m_WatchedFiles;

		static inline bool m_IsEnabled = false;
	};
}

