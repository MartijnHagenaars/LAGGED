#pragma once
#include "Utility/String.h"

namespace LAG
{
	class Resource
	{
		friend class ResourceManager;
	public: 
		Resource() = delete;
		explicit Resource(const Utility::String& path) : m_Path(path) {};
		virtual ~Resource() {};

		virtual bool Reload()
		{
			return (Unload() && Load());
		}

	protected:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;

		const Utility::String& GetPath() const { return m_Path; }

	private:
		const Utility::String& m_Path;
	};
}