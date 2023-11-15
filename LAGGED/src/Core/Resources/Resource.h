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

	protected:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;

		const HashedString& GetPath() const;

	private:
		const HashedString& m_Path;
	};
}