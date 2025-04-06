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

			size_t targetSize = (newArchetype->entityIDs.size() * sizeof(Comp)) + sizeof(Comp);
			if (newArchetype->compDataSize[0] <= targetSize)
			{
				ResizeAndReallocateComponentBuffer(*newArchetype, *compData, 0, targetSize);
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
				ComponentData* newCompData = m_ComponentMap.at(compID);

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
						ComponentData* oldCompData = m_ComponentMap.at(oldArchetype->typeID[i]);
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
				ShrinkComponentBuffer(*oldArchetype, entityRecord);
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

		const auto& compDataIt = m_ComponentMap.find(compID);
		if (compDataIt == m_ComponentMap.end())
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
			printf("Info: Creating Archetype with %i components: (%s)\n", newArchetype->typeID.size(), newArchetype->debugName.c_str());
#endif
		}

		for (int compIndex = 0; compIndex < newArchetypeID.size(); compIndex++)
		{
			const ComponentID newCompID = newArchetypeID[compIndex];
			ComponentData* newComp = m_ComponentMap.at(newCompID);

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
					size_t oldCompDataSize = m_ComponentMap.at(oldCompId)->size;
					ComponentData* oldCompData = m_ComponentMap.at(oldCompId);

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
					const auto& entityRecordIt = m_EntityArchetypes.find(entity);
					if (entityRecordIt == m_EntityArchetypes.end())
					{
						CRITICAL("Invalid Entity record.");
						return;
					}

					func(entity, (reinterpret_cast<Comps*>(&archetype->compData[GetSystemCompIndex(GetComponentID<Comps>(), archetype->typeID)][entityRecordIt->second.index * sizeof(Comps)]))...);
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
		newCompData->CreateObjectInMemory = [](unsigned char* dest) { new (&dest[0]) Comp(); };
		newCompData->MoveData = [](unsigned char* src, unsigned char* dest)
			{ 
				Comp* srcComp = reinterpret_cast<Comp*>(src);
				new (&dest[0]) Comp(std::move(*srcComp));
				srcComp->~Comp();
			};
		newCompData->DestructData = [](unsigned char* data)
			{
				Comp* comp = reinterpret_cast<Comp*>(data);
				comp->~Comp(); 
			};
#ifdef DEBUG
		newCompData->debugName = typeid(Comp).name();
#endif

		m_ComponentMap.insert({ compID, newCompData });
		return newCompData;
	}
}