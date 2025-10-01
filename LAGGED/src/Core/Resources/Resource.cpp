#include "Resource.h"

#include "ResourceManager.h"

namespace LAG
{
	Resource::Resource(const StringHash& path)
		: m_Path(path)
	{};

	bool Resource::Reload()
	{
		return (Unload() && Load());
	}

	const StringHash& Resource::GetPath() const
	{
		return m_Path;
	}
}