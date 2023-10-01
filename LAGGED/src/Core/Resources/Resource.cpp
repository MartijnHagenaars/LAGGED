#include "Precomp.h"
#include "Resource.h"

#include "Utility/String.h"
#include "ResourceManager.h"

namespace LAG
{
	Resource::Resource(const Utility::String& path)
		: m_Path(path)
	{};

	bool Resource::Reload()
	{
		return (Unload() && Load());
	}

	const Utility::String& Resource::GetPath() const
	{
		return m_Path;
	}
}