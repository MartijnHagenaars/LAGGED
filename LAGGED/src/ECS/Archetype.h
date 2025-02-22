#pragma once
#include <vector>
#include <string>

#include "ECS/TypeDefines.h"

namespace LAG
{
	struct Archetype
	{
#ifdef DEBUG
		std::string debugName;
#endif

		ArchetypeID typeID;
		std::vector<EntityID> entityIDs;

		std::vector<unsigned char*> compData;
		std::vector<uint64_t> compDataSize;
	};
}