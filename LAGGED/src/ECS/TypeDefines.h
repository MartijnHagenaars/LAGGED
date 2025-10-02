#pragma once
#include <cstdint>
#include <vector>

namespace LAG
{
	typedef size_t TypeID;

	typedef std::vector<TypeID> ArchetypeID;

	typedef TypeID EntityID;
	const EntityID ENTITY_NULL = 0;
}