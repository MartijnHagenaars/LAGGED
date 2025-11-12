#pragma once

#include <any>
#include <functional>
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

		std::unordered_map<TypeID, size_t> systemCompIndices; //Determines the order of components when running a system
	};

	struct TypeInfo
	{
#ifdef DEBUG
		std::string debugName;
#endif
		size_t size = 0;

		void(*Construct)(unsigned char* dest) = nullptr;
		void(*Destruct)(unsigned char* data) = nullptr;
		void(*Move)(unsigned char* src, unsigned char* dest) = nullptr;

		std::any(*VoidToAny)(void*) = nullptr;
		std::unordered_map<Hash64, std::function<void(const std::vector<std::any>&)>> funcs;
	};

	struct ReflectionCompInfo
	{
		struct MemberInfo
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
		std::vector<MemberInfo> members;

		struct Properties
		{
			bool isHidden = false;
			std::string displayName;
		} props;
	};
}