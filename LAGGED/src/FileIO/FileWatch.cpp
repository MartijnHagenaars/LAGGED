#include "FileWatch.h"
#include "Utility/Logger.h"

namespace LAG
{
#ifdef DEBUG
	void FileWatch::Register(FileIO::Directory dir, const std::string& relPath, std::function<void(const std::string&)> watchCallback)
	{
		const std::string& filePath = FileIO::GetPath(dir, relPath);
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

	void FileWatch::Remove(FileIO::Directory dir, const std::string& relPath)
	{
		const std::string& filePath = FileIO::GetPath(dir, relPath);
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

	bool FileWatch::IsWatchingFile(FileIO::Directory dir, const std::string& relPath)
	{
		for (const auto& it : m_WatchedFiles)
			if (it.filePath == FileIO::GetPath(dir, relPath))
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
	void FileWatch::Register(FileIO::Directory dir, const std::string& relPath, std::function<void(const std::string&)> watchCallback)
	{
	}

	void FileWatch::Remove(FileIO::Directory dir, const std::string& relPath)
	{
	}

	bool FileWatch::IsWatchingFile(FileIO::Directory dir, const std::string& relPath)
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