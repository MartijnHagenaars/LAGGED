#pragma once
#include "Utility/Hash.h"

namespace LAG
{
	class Resource
	{
		friend class ResourceManager;
	public: 
		Resource() = delete;
		explicit Resource(const StringHash64& path);
		virtual ~Resource() {};

		virtual bool Load() = 0;
		virtual bool Unload() = 0;
		virtual bool Reload();

		bool IsLoaded() const { return m_Loaded; }
		const StringHash64& GetPath() const;

	protected:
		void SetPath(const StringHash64& path) { m_Path = path; }
		void SetLoaded(bool loaded) { m_Loaded = loaded; }

	private:
		bool m_Loaded = false;
		StringHash64 m_Path;
	};
}