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

		const auto& compDataIt = m_ComponentMap.find(newCompID);
		if (compDataIt == m_ComponentMap.end())
		{
			compData = RegisterComponent<Comp>();
			INFO("Component {} not registered. Registering with ID {}...", typeid(Comp).name(), newCompID);
		}
		else compData = compDataIt->second;

		const auto& entArchIt = m_EntityArchetypes.find(entityID);
		if (entArchIt == m_EntityArchetypes.end())
			CRITICAL("Failed to find Entity with ID {} in EntityArchetype lookup map.", entityID);

		ArchetypeRecord& record = entArchIt->second;
		Archetype* oldArchetype = record.archetype;
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

			uint64_t targetSize = (newArchetype->entityIDs.size() * sizeof(Comp)) + sizeof(Comp);
			if (newArchetype->compDataSize[0] <= targetSize)
			{
#ifdef DEBUG
				INFO("Archetype component buffer is too small. Allocating more memory ({} -> {}) for ({})...",
					newArchetype->compDataSize[0],
					(newArchetype->compDataSize[0] * ARCHETYPE_ALLOC_GROWTH) + sizeof(Comp),
					newArchetype->debugName
				);
#endif

				newArchetype->compDataSize[0] = (newArchetype->compDataSize[0] * ARCHETYPE_ALLOC_GROWTH) + sizeof(Comp);
				unsigned char* newDataBuffer = new unsigned char[newArchetype->compDataSize[0]];
				for (int i = 0; i < newArchetype->entityIDs.size(); i++)
				{
					compData->moveData(&newArchetype->compData[0][i * sizeof(Comp)], &newDataBuffer[i * sizeof(Comp)]);
				}
				delete[] newArchetype->compData[0];
				newArchetype->compData[0] = newDataBuffer;
			}

			// Construct a new Comp object (with the correct Args) and place it into the component data array
			uint64_t compDataIndex = newArchetype->entityIDs.size() * sizeof(Comp);
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
				const ComponentID& compID = newArchetypeID[compIndex];
				const ComponentData* newCompData = m_ComponentMap[compID];
				const uint64_t newCompDataSize = newCompData->size;

				uint64_t currentSize = newArchetype->entityIDs.size() * newCompDataSize;
				uint64_t newSize = currentSize + newCompDataSize;

				// If there is no more space left for a component in a archetype, allocate some more and move all the previous 
				// component data from the old archetype buffer to the new one. 
				if (newSize > newArchetype->compDataSize[compIndex])
				{
					const uint64_t targetSize = (newArchetype->compDataSize[compIndex] * ARCHETYPE_ALLOC_GROWTH) + newCompDataSize;
#ifdef DEBUG
					INFO("Archetype component buffer is too small. Allocating more memory ({} -> {}) for ({})...",
						newArchetype->compDataSize[compIndex],
						targetSize, newArchetype->debugName);
#endif
					newArchetype->compDataSize[compIndex] = targetSize;
					unsigned char* newData = new unsigned char[newArchetype->compDataSize[compIndex]];
					//If this archetype has entities, we need to loop through all entities and move their data from the old (smaller) to the new (larger) buffer
					for (int entIndex = 0; entIndex < newArchetype->entityIDs.size(); ++entIndex)
					{
						newCompData->moveData(&newArchetype->compData[compIndex][entIndex * newCompDataSize], &newData[entIndex * newCompDataSize]);
					}
					//The data has been moved so we can delete the old buffer and replace it with the new one.
					delete[] newArchetype->compData[compIndex];
					newArchetype->compData[compIndex] = newData;
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
						ComponentData* oldCompData = m_ComponentMap[oldArchetype->typeID[i]];
						oldCompData->moveData(&oldArchetype->compData[i][record.index * oldCompData->size], &newArchetype->compData[compIndex][currentSize]);
						break;
					}
					else if (i == (oldArchetype->typeID.size() - 1) && !matchingIDs)
					{
						newComponent = new (&newArchetype->compData[compIndex][currentSize]) Comp(std::forward<Args>(compArgs)...);
					}
				}
			}

			// If a new component is added, the data buffer for the old archetype needs to be shrunk.
			if (!oldArchetype->entityIDs.empty())
			{
				for (int typeIndex = 0; typeIndex < oldArchetype->typeID.size(); typeIndex++)
				{
					const ComponentData* oldCompData = m_ComponentMap[oldArchetype->typeID[typeIndex]];

					unsigned char* newData;
					const size_t oldCompDataSize = oldCompData->size;
					const size_t targetSize = (oldArchetype->entityIDs.size() * oldCompDataSize) - oldCompDataSize;

					if (targetSize > 0)
					{
#ifdef DEBUG
						INFO("Archetype component buffer is too large. Shrinking buffer memory ({} -> {}) for ({})...",
							newArchetype->compDataSize[typeIndex],
							oldArchetype->compDataSize[typeIndex] - oldCompDataSize,
							newArchetype->debugName);
#endif
						newData = new unsigned char[oldArchetype->compDataSize[typeIndex] - oldCompDataSize];
						oldArchetype->compDataSize[typeIndex] -= oldCompDataSize;

						int offsetIndex = 0;
						for (int entIndex = 0; entIndex < oldArchetype->entityIDs.size(); ++entIndex)
						{
							if (entIndex == record.index)
								continue;

							oldCompData->moveData(&oldArchetype->compData[typeIndex][entIndex * oldCompDataSize], &newData[offsetIndex * oldCompDataSize]);
							++offsetIndex;
						}
					}
					else
					{
#ifdef DEBUG
						INFO("Deallocating component buffer ({} -> {}) for ({})...",
							oldArchetype->compDataSize[typeIndex],
							0, newArchetype->debugName);
#endif
						newData = nullptr;
						oldArchetype->compDataSize[typeIndex] = 0;
					}

					delete[] oldArchetype->compData[typeIndex];
					oldArchetype->compData[typeIndex] = newData;
				}

				int decrementStartingIndex = -1;
				for (int i = 0; i < oldArchetype->entityIDs.size(); i++)
				{
					if (decrementStartingIndex == -1 && oldArchetype->entityIDs[i] == entityID)
						decrementStartingIndex = i;

					if (decrementStartingIndex != -1)
					{
						ArchetypeRecord& record = m_EntityArchetypes[oldArchetype->entityIDs[i]];
						record.index -= 1;
					}
				}
				if (decrementStartingIndex != -1)
					oldArchetype->entityIDs.erase(oldArchetype->entityIDs.begin() + decrementStartingIndex);
			}
		}

		// Update the entity archetype record and add the EntityID to the archetype. 
		newArchetype->entityIDs.push_back(entityID);

		record.archetype = newArchetype;
		record.index = newArchetype->entityIDs.size() - 1;
		return newComponent;
	}

	template<typename Comp>
	inline bool Scene::HasComponent(const EntityID& entityID)
	{
		return Entity(*this, entityID).HasComponent<Comp>();
	}

	template<typename Comp>
	inline Comp* Scene::GetComponent(const EntityID& entityID)
	{
		return Entity(*this, entityID).GetComponent<Comp>();
	}

	template<typename ...Comps>
	inline void Scene::RunSystem(std::function<void(EntityID, Comps*...)> func)
	{
		if (func == nullptr)
		{
			CRITICAL("Function pointer in RunSystem is invalid.");
			return;
		}

		//Consider moving this into a private function in Scene class
		const auto& GetSystemCompIndex =
			[](ComponentID compID, const std::vector<EntityID>& entityIDs)
			{
				for (int i = 0; i < entityIDs.size(); i++)
					if (compID == entityIDs[i])
						return i;

				CRITICAL("Failed to get System Component Index: No ComponentID matches {}.", compID);
				return -1;
			};

		//Create key
		ArchetypeID archetypeID = { { GetComponentID<Comps>()... } };
		std::sort(archetypeID.begin(), archetypeID.end());

		for (Archetype* archetype : m_Archetypes)
		{
			if (std::includes(archetype->typeID.begin(), archetype->typeID.end(), archetypeID.begin(), archetypeID.end()))
			{
				for (const auto& entity : archetype->entityIDs)
				{
					const auto& archRecord = m_EntityArchetypes.find(entity);
					if (archRecord == m_EntityArchetypes.end())
					{
						CRITICAL("Invalid archetype record.");
						return;
					}

					func(entity, (reinterpret_cast<Comps*>(&archetype->compData[GetSystemCompIndex(GetComponentID<Comps>(), archetype->typeID)][archRecord->second.index * sizeof(Comps)]))...);
				}
			}
		}
	}

	template<typename ...Comps>
	inline std::vector<EntityID> Scene::GetEntitiesWithComponents()
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
	inline Scene::ComponentData* Scene::RegisterComponent()
	{
		ComponentID compID = GetComponentID<Comp>();

		//Check if component has already been registered. 
		const auto& compDataIt = m_ComponentMap.find(compID);
		if (compDataIt != m_ComponentMap.end())
			return compDataIt->second;

		// Create and insert new data into the m_ComponentMap map
		ComponentData* newCompData = new ComponentData;
		newCompData->size = sizeof(Comp);
		newCompData->moveData = [](unsigned char* src, unsigned char* dest)
			{ new (&dest[0]) Comp(std::move(*reinterpret_cast<Comp*>(src))); };
#ifdef DEBUG
		newCompData->debugName = typeid(Comp).name();
#endif

		m_ComponentMap.insert({ compID, newCompData });
		return newCompData;
	}
}