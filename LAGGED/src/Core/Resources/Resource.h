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

	protected:
		void SetPath(const HashedString& path) { m_Path = path; }

	private:
		bool m_Loaded = false;
		HashedString m_Path;
	};
}