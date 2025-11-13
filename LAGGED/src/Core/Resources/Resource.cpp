#include "Resource.h"

#include "ResourceManager.h"

namespace LAG
{
	Resource::Resource(const StringHash64& path)
		: m_Path(path)
	{};

	bool Resource::Reload()
	{
		return (Unload() && Load());
	}

	const StringHash64& Resource::GetPath() const
	{
		return m_Path;
	}
}