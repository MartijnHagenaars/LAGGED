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
		//Loop over all elements and delete them. 
		for (auto it = m_Resources.begin(); it != m_Resources.end();)
		{
			it->second->Unload();
			it->second.reset();
			m_Resources.erase(it++->first);
		}

		m_Resources.clear();
	}
}