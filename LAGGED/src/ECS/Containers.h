#pragma once

#include <any>
#include <string>
#include <unordered_map>
#include <vector>

#include "ECS/TypeDefines.h"
#include "Utility/Hash.h"

namespace LAG
{
	struct Archetype
	{
#ifdef DEBUG
		std::string debugName;
#endif

		ArchetypeID typeID; //ID of the archetype
		std::vector<EntityID> entityIDs; //EntityIDs that belong to this archetype

		std::vector<unsigned char*> compData; //The data of all the components
		std::vector<size_t> compDataSize; //The size of all components

		std::unordered_map<ComponentID, size_t> systemCompIndices; //Determines the order of components when running a system
	};

	struct ComponentData
	{
		size_t size = -1;

		void(*CreateObjectInMemory)(unsigned char* dest) = nullptr;
		void(*MoveData)(unsigned char* src, unsigned char* dest) = nullptr;
		void(*DestructData)(unsigned char* data) = nullptr;

		std::any(*VoidToAny)(void*) = nullptr;
#ifdef DEBUG
		std::string debugName;
#endif
	};

	struct ReflectionData
	{
		struct MemberData
		{
			Hash64 typeID = 0;
			size_t byteOffset = -1;
			struct Properties
			{
				bool isHidden = false;
				bool isReadOnly = false;
				std::string displayName;
			} props;
		};
		std::vector<MemberData> members;

		struct Properties
		{
			bool isHidden = false;
			std::string displayName;
		} props;
	};

	struct ReflectionTypesData
	{
		std::any(*VoidToAny)(void*) = nullptr;
	};
}