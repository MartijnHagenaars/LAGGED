#pragma once
#include "Utility/String.h"

namespace LAG
{
	class Resource
	{
		friend class ResourceManager;
	public: 
		Resource() = delete;
		explicit Resource(const Utility::String& path);
		virtual ~Resource() {};

		virtual bool Reload();

	protected:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;

		const Utility::String& GetPath() const;

	private:
		const Utility::String& m_Path;
	};
}