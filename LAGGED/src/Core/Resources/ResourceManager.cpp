#include "ResourceManager.h"

namespace LAG
{
	ResourceManager::ResourceManager()
	{}

	ResourceManager::~ResourceManager()
	{
		Clear();
	}

	void ResourceManager::Clear()
	{
		if (m_Resources.empty())
			return;

		for (auto it = m_Resources.begin(); it != m_Resources.end();)
		{
			it->second->Unload();
			m_Resources.erase(it++);
		}

		m_Resources.clear();
	}
}