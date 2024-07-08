#include "FileWatch.h"
#include "Utility/Logger.h"

namespace LAG
{
#ifdef DEBUG
	void FileWatch::Register(const std::string& filePath, std::function<void(const std::string&)> watchCallback)
	{
		if (watchCallback == nullptr)
		{
			Logger::Error("Callback function for {0} is nullptr.", filePath);
			return;
		}

		for (auto&& it : m_WatchedFiles)
		{
			if (filePath == it.filePath)
			{
				Logger::Error("File watch already exists for {0}.", filePath);
				return;
			}
		}

		WatchData wd;
		wd.filePath = filePath;
		wd.callbackFunc = watchCallback;
		wd.lastFileTime = std::filesystem::last_write_time(filePath);

		m_WatchedFiles.emplace_back(wd);
	}

	void FileWatch::Remove(const std::string& filePath)
	{
		for (auto it = m_WatchedFiles.begin(); it != m_WatchedFiles.end();)
		{
			if (filePath == it->filePath)
			{
				m_WatchedFiles.erase(it);
				return;
			}
			++it;
		}

		Logger::Warning("Tried removing file watch entry that does not exist: {0}", filePath);
	}

	bool FileWatch::IsWatchingFile(const std::string& filePath)
	{
		for (const auto& it : m_WatchedFiles)
			if (it.filePath == filePath)
				return true;

		return false;
	}

	void FileWatch::CheckWatches()
	{
		if (!m_IsEnabled)
			return;

		for (auto&& it : m_WatchedFiles)
		{
			auto writeTime = std::filesystem::last_write_time(it.filePath);
			if (it.lastFileTime < writeTime)
			{
				it.lastFileTime = writeTime;
				it.callbackFunc(it.filePath);
			}
		}
	}

	void FileWatch::EnableFileWatch(bool enable)
	{
		m_IsEnabled = enable;
	}

#else
	void FileWatch::Register(const std::string& filePath, std::function<void(const std::string&)> watchCallback)
	{
	}

	void FileWatch::Remove(const std::string& filePath)
	{
	}

	bool FileWatch::IsWatchingFile(const std::string& filePath)
	{
		return false;
	}

	void FileWatch::CheckWatches()
	{
	}

	void FileWatch::EnableFileWatch(bool enable)
	{
	}
#endif
}