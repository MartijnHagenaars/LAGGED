#include "Scene.h"
#pragma once
#define ARCHETYPE_ALLOC_GROWTH 2

namespace LAG
{
	template<typename Comp, typename ...Args>
	inline Comp* Scene::AddComponent(const EntityID entityID, Args && ...compArgs)
	{
		ComponentData* compData = nullptr;
		const ComponentID compID = GetComponentID<Comp>();

		const auto& compDataIt = m_ComponentMap.find(compID);
		if (compDataIt == m_ComponentMap.end())
		{
			compData = RegisterComponent<Comp>();
			INFO("Component {} not registered. Registering with ID {}...", typeid(Comp).name(), compID);
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
			ArchetypeID newArchetypeID = { compID };

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

			int targetSize = (newArchetype->entityIDs.size() * sizeof(Comp)) + sizeof(Comp);
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
			int compDataIndex = newArchetype->entityIDs.size() * sizeof(Comp);
			newComponent = new(&newArchetype->compData[0][compDataIndex])  Comp(std::forward<Args>(compArgs)...);
		}


		// Update the entity archetype record and add the EntityID to the archetype. 
		newArchetype->entityIDs.push_back(entityID);

		record.archetype = newArchetype;
		record.index = newArchetype->entityIDs.size() - 1;
		return newComponent;
	}


	inline int GetSystemCompIndex(ComponentID compID, const std::vector<EntityID>& entityIDs)
	{
		for (int i = 0; i < entityIDs.size(); i++)
		{
			if (compID == entityIDs[i])
				return i;
		}

		CRITICAL("Failed to get System Component Index: No ComponentID matches {}.", compID);
		return -1;
	}

	template<typename ...Comps>
	inline void Scene::RunSystem(std::function<void(EntityID, Comps*...)> func)
	{
		if (func == nullptr)
		{
			CRITICAL("Function pointer in RunSystem is invalid.");
			return;
		}

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