#pragma once
#include "Utility/HashedString.h"

namespace LAG
{
	class Resource
	{
		friend class ResourceManager;
	public: 
		Resource() = delete;
		explicit Resource(const HashedString& path);
		virtual ~Resource() {};

		virtual bool Load() = 0;
		virtual bool Unload() = 0;
		virtual bool Reload();

		bool IsLoaded() const { return m_Loaded; }
		const HashedString& GetPath() const;

	private:
		bool m_Loaded = false;
		const HashedString m_Path;
	};
}