#include "Scene.h"
#pragma once
#define ARCHETYPE_ALLOC_GROWTH 2

namespace LAG
{
	template<typename Comp, typename ...Args>
	inline Comp* Scene::AddComponent(const EntityID entityID, Args && ...compArgs)
	{
		// Get the component data structure.
		constexpr TypeID typeID = GetTypeID<Comp>();
		if (auto typeInfoIt = GetTypeInfo().find(typeID); typeInfoIt == GetTypeInfo().end())
		{
			// Register component in the case it hasn't been done yet.
			RegisterType<Comp>();
			WARNING("Registering component {} (ID: {}) at runtime.", typeid(Comp).name(), typeID);
		}

		const auto& entityRecordIt = m_EntityArchetypes.find(entityID);
		if (entityRecordIt == m_EntityArchetypes.end())
			CRITICAL("Failed to find Entity with ID {} in EntityArchetype lookup map.", entityID);

		void* compPtr = AddComponent(entityID, typeID);
		if constexpr (sizeof...(compArgs) > 0)
		{
			new(compPtr)  Comp(std::forward<Args>(compArgs)...);
		}

		return reinterpret_cast<Comp*>(compPtr);
	}

	template<typename Comp>
	inline void Scene::RemoveComponent(const EntityID entityID)
	{
		const auto& entityRecordIt = m_EntityArchetypes.find(entityID);
		if (entityRecordIt == m_EntityArchetypes.end())
			CRITICAL("Failed to remove component: Record for entity ({}) could not be found.", entityID);

		EntityRecord& entityRecord = entityRecordIt->second;
		Archetype* oldArchetype = entityRecord.archetype;
		// If oldArchetype is nullptr, it doesnt contain any components.
		if (oldArchetype == nullptr)
			return;

		// Check if this entity contains the component.
		const ArchetypeID::iterator& archetypeIdIt = std::find(oldArchetype->typeID.begin(), oldArchetype->typeID.end(), GetTypeID<Comp>());
		if (archetypeIdIt == oldArchetype->typeID.end())
		{
			WARNING("Failed to remove component: Entity ({}) does not contain {} component.", entityID, typeid(Comp).name());
			return;
		}

		// Remove the old TypeID and construct the new ArchetypeID
		ArchetypeID newArchetypeID = oldArchetype->typeID;
		newArchetypeID.erase(newArchetypeID.begin() + (archetypeIdIt - oldArchetype->typeID.begin()));
		std::sort(newArchetypeID.begin(), newArchetypeID.end());

		Archetype* newArchetype = GetArchetype(newArchetypeID);
		if (!newArchetype)
		{
			newArchetype = CreateArchetype(newArchetypeID);
#ifdef DEBUG
			newArchetype->debugName = typeid(Comp).name();
			INFO("Creating archetype with {} components: {}", newArchetype->typeID.size(), newArchetype->debugName.c_str());
#endif
		}

		for (int compIndex = 0; compIndex < newArchetypeID.size(); compIndex++)
		{
			const TypeID newTypeID = newArchetypeID[compIndex];
			const TypeInfo& newComp = GetTypeInfo().at(newTypeID);

			size_t newCompDataSize = newComp.size;
			size_t currentSize = newArchetype->entityIDs.size() * newCompDataSize;
			size_t newSize = currentSize + newCompDataSize;

			// If there is not enough space in the new archetype, 
			// allocate some more memory and move the component data 
			// from the previous archetype to the new one.
			if (newSize > newArchetype->compDataSize[compIndex])
			{
				size_t targetSize = (newArchetype->compDataSize[compIndex] * ARCHETYPE_ALLOC_GROWTH) + newCompDataSize;
				ResizeAndReallocateComponentBuffer(*newArchetype, newComp, compIndex, targetSize);
			}

			newComp.Construct(&newArchetype->compData[compIndex][currentSize]);

			ArchetypeID oldArchetypeId = oldArchetype->typeID;
			for (size_t i = 0; i < oldArchetype->typeID.size(); i++)
			{
				TypeID oldTypeId = oldArchetype->typeID[i];

				if (oldTypeId == newTypeID)
				{
					const TypeInfo& oldComp = GetTypeInfo().at(oldTypeId);
					oldComp.Move(&oldArchetype->compData[i][entityRecord.index * oldComp.size], &newArchetype->compData[compIndex][currentSize]);
					break;
				}
			}
		}

		if (!oldArchetype->entityIDs.empty())
		{
			ShrinkComponentBuffer(*oldArchetype, entityRecord);
			RemoveEntityFromArchetype(entityID, *oldArchetype);
		}

		newArchetype->entityIDs.push_back(entityID);
		entityRecord.index = newArchetype->entityIDs.size() - 1;
		entityRecord.archetype = newArchetype;
	}

	template<typename Comp>
	inline bool Scene::HasComponent(const EntityID& entityID)
	{
		const auto& recordIt = m_EntityArchetypes.find(entityID);
		if (recordIt == m_EntityArchetypes.end())
			return false;

		Archetype* archetype = recordIt->second.archetype;
		constexpr TypeID typeID = Scene::GetTypeID<Comp>();
		return (std::find(archetype->typeID.begin(), archetype->typeID.end(), typeID)) != archetype->typeID.end();
	}

	template<typename Comp>
	inline Comp* Scene::GetComponent(const EntityID& entityID)
	{
		if (entityID == ENTITY_NULL)
		{
			CRITICAL("Failed to run GetComponent: EntityID is null.");
			return nullptr;
		}

		const auto& recordIt = m_EntityArchetypes.find(entityID);
		if (recordIt == m_EntityArchetypes.end() || recordIt->second.archetype == nullptr)
		{
			CRITICAL("Failed to run GetComponent for Entity {}: Record is null.", entityID);
			return nullptr;
		}

		int dataOffset = 0;
		const Scene::EntityRecord& record = recordIt->second;
		Archetype* archetype = record.archetype;

		for (int i = 0; i < archetype->typeID.size(); i++)
		{
			if (archetype->typeID[i] == Scene::GetTypeID<Comp>())
				return reinterpret_cast<Comp*>(&archetype->compData[dataOffset][record.index * sizeof(Comp)]);
			else
				++dataOffset;
		}

		return nullptr;
	}

	template<typename ...Comps>
	typename std::enable_if<(sizeof...(Comps) > 0), void>::type
		inline Scene::ForEach(std::function<void(EntityID, Comps*...)> func)
	{
		// Create the archetypeID key. This is only executed once for this specific template type...
		static const ArchetypeID archetypeID = []()
			{
				ArchetypeID id = { { GetTypeID<Comps>()... } };
				std::sort(id.begin(), id.end());
				return id;
			}();

		for (Archetype* archetype : m_Archetypes)
		{
			if (std::includes(archetype->typeID.begin(), archetype->typeID.end(), archetypeID.begin(), archetypeID.end()))
			{
				for (const auto& entity : archetype->entityIDs)
				{
					const auto& entityRecordIt = m_EntityArchetypes.find(entity);
#ifdef DEBUG
					if (entityRecordIt == m_EntityArchetypes.end())
					{
						CRITICAL("Invalid Entity record.");
						return;
					}
#endif

					func(entity,
						(reinterpret_cast<Comps*>(
							&archetype->compData
							[archetype->systemCompIndices[GetTypeID<Comps>()]]
							[entityRecordIt->second.index * sizeof(Comps)]))...);
				}
			}
		}
	}

	template<typename ...Comps>
	typename std::enable_if<(sizeof...(Comps) == 0), void>::type
		inline Scene::ForEach(std::function<void(EntityID)> func)
	{
		for (Archetype* archetype : m_Archetypes)
		{
			for (EntityID entID : archetype->entityIDs)
				func(entID);
		}
	}

	template<typename ...Comps>
	inline std::vector<EntityID> Scene::QueryEntities()
	{
		//Create key
		ArchetypeID archetypeID = { { GetTypeID<Comps>()... } };
		std::sort(archetypeID.begin(), archetypeID.end());

		std::vector<EntityID> results;
		for (Archetype* archetype : m_Archetypes)
		{
			if (std::includes(archetype->typeID.begin(), archetype->typeID.end(), archetypeID.begin(), archetypeID.end()))
			{
				for (EntityID entity : archetype->entityIDs)
				{
					if (entity != ENTITY_NULL)
						results.emplace_back(entity);
				}
			}
		}

		return results;
	}

	template<typename T>
	inline constexpr TypeID Scene::GetTypeID()
	{
		return GetTypeHash64<T>();
	}

	template<typename Type>
	inline TypeInfo& Scene::RegisterType()
	{
		constexpr TypeID typeID = GetTypeID<Type>();

		// Check if component has already been registered. 
		if (const auto& typeInfoIt = GetTypeInfo().find(typeID); typeInfoIt != GetTypeInfo().end())
			return typeInfoIt->second;

		// Create and insert new data into the GetTypeInfo() map
		TypeInfo newTypeInfo = {};

		newTypeInfo.size = sizeof(Type);
#ifdef DEBUG
		newTypeInfo.debugName = typeid(Type).name();
#endif

		newTypeInfo.Construct = [](unsigned char* dest) { new (&dest[0]) Type(); };



		newTypeInfo.Destruct = [](unsigned char* data) { reinterpret_cast<Type*>(data)->~Type(); };
		newTypeInfo.Move = [](unsigned char* src, unsigned char* dest) { new (&dest[0]) Type(std::move(*reinterpret_cast<Type*>(src))); };
		newTypeInfo.VoidToAny = [](void* data) { return std::any(static_cast<Type*>(data)); };

		return GetTypeInfo().insert({ typeID, newTypeInfo }).first->second;
	}
}