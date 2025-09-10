#include "Scene.h"
#pragma once
#define ARCHETYPE_ALLOC_GROWTH 2

namespace LAG
{
	template<typename Comp, typename ...Args>
	inline Comp* Scene::AddComponent(const EntityID entityID, Args && ...compArgs)
	{
		ComponentData* compData = nullptr;
		const ComponentID newCompID = GetComponentID<Comp>();

		// Get the component data structure.
		if (auto compDataIt = s_ComponentMap.find(newCompID); compDataIt != s_ComponentMap.end())
			compData = compDataIt->second;
		else 
		{
			// Register component in the case it hasn't been done yet.
			compData = RegisterComponent<Comp>();
			WARNING("Registering component {} (ID: {}) at runtime.", typeid(Comp).name(), newCompID);
		}

		const auto& entityRecordIt = m_EntityArchetypes.find(entityID);
		if (entityRecordIt == m_EntityArchetypes.end())
			CRITICAL("Failed to find Entity with ID {} in EntityArchetype lookup map.", entityID);

		EntityRecord& entityRecord = entityRecordIt->second;
		Archetype* oldArchetype = entityRecord.archetype;
		Archetype* newArchetype = nullptr;
		Comp* newComponent = nullptr;

		// When oldArchetype is equal to nullptr, the entity doesn't hold any component data.
		// If oldArchetype is NOT equal to nullptr, the entity already holds onto some component data.
		if (oldArchetype == nullptr)
		{
			//This is the first time we're adding a component to this entity. 
			//Create a new archetypeID which contains this component ID
			ArchetypeID newArchetypeID = { newCompID };

			newArchetype = GetArchetype(newArchetypeID);
			if (newArchetype == nullptr)
			{
				newArchetype = CreateArchetype(newArchetypeID);
#ifdef DEBUG
				newArchetype->debugName = typeid(Comp).name();
				INFO("Creating Archetype with {} component : ({})",
					newArchetype->typeID.size(), newArchetype->debugName
				);
#endif
			}

			//size_t targetSize = (newArchetype->entityIDs.size() * sizeof(Comp)) + sizeof(Comp);
			size_t targetSize = (newArchetype->entityIDs.size() * sizeof(Comp)) + sizeof(Comp);
			if (newArchetype->compDataSize[0] <= targetSize)
			{
				size_t newSize = (newArchetype->compDataSize[0] * ARCHETYPE_ALLOC_GROWTH) + sizeof(Comp);
				ResizeAndReallocateComponentBuffer(*newArchetype, *compData, 0, newSize);
			}

			// Construct a new Comp object (with the correct Args) and place it into the component data array
			size_t compDataIndex = newArchetype->entityIDs.size() * sizeof(Comp);
			newComponent = new(&newArchetype->compData[0][compDataIndex])  Comp(std::forward<Args>(compArgs)...);
		}
		else
		{
			//Check if this component already exists on the entity.
			//If that's the case, log a warning and return the component
			if (std::find(oldArchetype->typeID.begin(), oldArchetype->typeID.end(), newCompID) != oldArchetype->typeID.end())
			{
				WARNING("Component ({}) already exists on entity with ID {}", typeid(Comp).name(), GetComponentID<Comp>());
				return GetComponent<Comp>(entityID);
			}

			ArchetypeID newArchetypeID = oldArchetype->typeID;
			newArchetypeID.push_back(newCompID);
			std::sort(newArchetypeID.begin(), newArchetypeID.end());

			newArchetype = GetArchetype(newArchetypeID);
			if (newArchetype == nullptr)
			{
				newArchetype = CreateArchetype(newArchetypeID);
#ifdef DEBUG
				newArchetype->debugName = oldArchetype->debugName + "|" + typeid(Comp).name();
				INFO("Creating Archetype with {} components: {}", newArchetype->typeID.size(), newArchetype->debugName);
#endif
			}

			// This for-loop goes over all components.
			// It handles allocating more space in the new archetype and moving the component data from the old archetype to the new one. 
			// In the end, after everything has been moved, the new component is added. 
			for (int compIndex = 0; compIndex < newArchetypeID.size(); compIndex++)
			{
				const ComponentID compID = newArchetypeID[compIndex];
				ComponentData* newCompData = s_ComponentMap.at(compID);

				size_t newCompDataSize = newCompData->size;
				size_t currentSize = newArchetype->entityIDs.size() * newCompDataSize;
				size_t newSize = currentSize + newCompDataSize;

				// If there is no more space left for a component in a archetype, allocate some more and move all the previous 
				// component data from the old archetype buffer to the new one. 
				if (newSize > newArchetype->compDataSize[compIndex])
				{
					size_t targetSize = (newArchetype->compDataSize[compIndex] * ARCHETYPE_ALLOC_GROWTH) + newCompDataSize;
					ResizeAndReallocateComponentBuffer(*newArchetype, *newCompData, compIndex, targetSize);
				}


				// In this loop, we move all the data from the old archetype to the new one. 
				// While doing this, the new component is also added to the new archetype at the correct index.
				ArchetypeID oldArchetypeId = oldArchetype->typeID;
				for (int i = 0; i < oldArchetype->typeID.size(); i++)
				{
					ComponentID oldCompID = oldArchetype->typeID[i];
					bool matchingIDs = (oldCompID == compID);
					if (matchingIDs)
					{
						ComponentData* oldCompData = s_ComponentMap.at(oldArchetype->typeID[i]);
						oldCompData->MoveData(&oldArchetype->compData[i][entityRecord.index * oldCompData->size], &newArchetype->compData[compIndex][currentSize]);
						break;
					}
					else if (i == (oldArchetype->typeID.size() - 1) && !matchingIDs)
					{
						newComponent = new (&newArchetype->compData[compIndex][currentSize]) Comp(std::forward<Args>(compArgs)...);
					}
				}
			}

			// If a new component is added, the data buffer 
			// for the old archetype needs to be shrunk.
			if (!oldArchetype->entityIDs.empty())
			{
				//ShrinkComponentBuffer(*oldArchetype, entityRecord);
				RemoveEntityFromArchetype(entityID, *oldArchetype);
			}
		}

		// Update the entity archetype record 
		// and add the EntityID to the archetype. 
		newArchetype->entityIDs.push_back(entityID);

		entityRecord.archetype = newArchetype;
		entityRecord.index = newArchetype->entityIDs.size() - 1;
		return newComponent;
	}

	template<typename Comp>
	inline void Scene::RemoveComponent(const EntityID entityID)
	{
		const ComponentID compID = GetComponentID<Comp>();

		const auto& compDataIt = s_ComponentMap.find(compID);
		if (compDataIt == s_ComponentMap.end())
			CRITICAL("Failed to remove component: Component {} is not registered.", typeid(Comp).name());

		const ComponentData* compDataPtr = compDataIt->second;
		const auto& entityRecordIt = m_EntityArchetypes.find(entityID);
		if (entityRecordIt == m_EntityArchetypes.end())
			CRITICAL("Failed to remove component: Record for entity ({}) could not be found.", entityID);

		EntityRecord& entityRecord = entityRecordIt->second;
		Archetype* oldArchetype = entityRecord.archetype;
		// If oldArchetype is nullptr, it doesnt contain any components.
		if (oldArchetype == nullptr)
			return;

		// Check if this entity contains the component.
		const ArchetypeID::iterator& archetypeIdIt = std::find(oldArchetype->typeID.begin(), oldArchetype->typeID.end(), compID);
		if (archetypeIdIt == oldArchetype->typeID.end())
		{
			WARNING("Failed to remove component: Entity ({}) does not contain {} component.", entityID, typeid(Comp).name());
			return;
		}

		// Remove the old ComponentID and construct the new ArchetypeID
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
			const ComponentID newCompID = newArchetypeID[compIndex];
			ComponentData* newComp = s_ComponentMap.at(newCompID);

			size_t newCompDataSize = newComp->size;
			size_t currentSize = newArchetype->entityIDs.size() * newCompDataSize;
			size_t newSize = currentSize + newCompDataSize;

			//If there is not enough space in the new archetype, 
			// allocate some more memory and move the component data 
			// from the previous archetype to the new one.
			if (newSize > newArchetype->compDataSize[compIndex])
			{
				size_t targetSize = (newArchetype->compDataSize[compIndex] * ARCHETYPE_ALLOC_GROWTH) + newCompDataSize;
				ResizeAndReallocateComponentBuffer(*newArchetype, *newComp, compIndex, targetSize);
			}

			newComp->CreateObjectInMemory(&newArchetype->compData[compIndex][currentSize]);

			ArchetypeID oldArchetypeId = oldArchetype->typeID;
			for (size_t i = 0; i < oldArchetype->typeID.size(); i++)
			{
				ComponentID oldCompId = oldArchetype->typeID[i];

				if (oldCompId == newCompID)
				{
					size_t oldCompDataSize = s_ComponentMap.at(oldCompId)->size;
					ComponentData* oldCompData = s_ComponentMap.at(oldCompId);

					oldCompData->MoveData(&oldArchetype->compData[i][entityRecord.index * oldCompDataSize], &newArchetype->compData[compIndex][currentSize]);

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
		ComponentID compID = Scene::GetComponentID<Comp>();
		return (std::find(archetype->typeID.begin(), archetype->typeID.end(), compID)) != archetype->typeID.end();
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
			if (archetype->typeID[i] == Scene::GetComponentID<Comp>())
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
				ArchetypeID id = { { GetComponentID<Comps>()... } };
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
							[archetype->systemCompIndices[GetComponentID<Comps>()]]
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
		ArchetypeID archetypeID = { { GetComponentID<Comps>()... } };
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

	template<typename Comp>
	inline const ComponentID Scene::GetComponentID()
	{
		static const ComponentID compID = s_ComponentCounter++;
		return compID;
	}

	template<typename Comp>
	inline ComponentData* Scene::RegisterComponent()
	{
		ComponentID compID = GetComponentID<Comp>();

		//Check if component has already been registered. 
		const auto& compDataIt = s_ComponentMap.find(compID);
		if (compDataIt != s_ComponentMap.end())
			return compDataIt->second;

		// Create and insert new data into the s_ComponentMap map
		ComponentData* newCompData = new ComponentData;
		newCompData->size = sizeof(Comp);
		newCompData->CreateObjectInMemory = [](unsigned char* dest) { new (&dest[0]) Comp(); };
		newCompData->MoveData = [](unsigned char* src, unsigned char* dest) { new (&dest[0]) Comp(std::move(*reinterpret_cast<Comp*>(src))); };
		newCompData->DestructData = [](unsigned char* data) { reinterpret_cast<Comp*>(data)->~Comp(); };
#ifdef DEBUG
		newCompData->debugName = typeid(Comp).name();
#endif

		s_ComponentMap.insert({ compID, newCompData });
		return newCompData;
	}
}