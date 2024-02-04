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

		virtual bool Reload();
		const HashedString& GetPath() const;

	protected:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;

	private:
		const HashedString m_Path;
	};
}