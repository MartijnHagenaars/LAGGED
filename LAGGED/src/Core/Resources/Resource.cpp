#include "Precomp.h"
#include "Resource.h"

#include "ResourceManager.h"

namespace LAG
{
	Resource::Resource(const HashedString& path)
		: m_Path(path)
	{};

	bool Resource::Reload()
	{
		return (Unload() && Load());
	}

	const HashedString& Resource::GetPath() const
	{
		return m_Path;
	}
}