#pragma once
#include <cstdint>
#include <vector>

namespace LAG
{
	typedef uint64_t TypeID;

	typedef TypeID ComponentID;
	typedef std::vector<ComponentID> ArchetypeID;

	typedef TypeID EntityID;
	const EntityID ENTITY_NULL = 0;
}