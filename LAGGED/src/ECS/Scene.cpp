#include "Scene.h"
#include "ECS/Entity.h"

#include "Components/BasicComponents.h"

namespace LAG
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		RemoveAll();
	}

	Entity Scene::AddEntity()
	{
		EntityID newEntityID = ++s_EntityCounter;

		auto entArchetype = m_EntityArchetypes.find(newEntityID);
		if (entArchetype != m_EntityArchetypes.end())
			ERROR("Entity with ID {} already exists in EnityArchetypes map.", newEntityID);

		EntityRecord rec = {};
		rec.index = 0;
		rec.archetype = nullptr;
		m_EntityArchetypes.insert({ newEntityID, rec });

		return Entity(*this, newEntityID);
	}

	Entity Scene::AddEntity(const std::string& entityName)
	{
		Entity newEntity = AddEntity();
		newEntity.AddComponent<DefaultComponent>(entityName);
		
		return newEntity;
	}

	Entity Scene::DuplicateEntity(EntityID entityID)
	{
		//TODO: Implement function...
		return Entity();
	}

	void Scene::RemoveEntity(EntityID entityID)
	{
		const auto& entityRecordIt = m_EntityArchetypes.find(entityID);
		if (entityRecordIt == m_EntityArchetypes.end())
		{
			ERROR("Tried to delete an entity with ID {}, which does not exist.", entityID);
			return;
		}
		EntityRecord& entityRecord = entityRecordIt->second;
		Archetype* entityArchetype = entityRecord.archetype;

		if (entityArchetype)
		{
			for (int i = 0; i < entityArchetype->typeID.size(); i++)
			{
				ComponentID compID = entityArchetype->typeID[i];
				ComponentData* compData = m_ComponentMap.at(compID);
				compData->DestructData(&entityArchetype->compData[i][entityRecord.index * compData->size]);
			}

			ShrinkComponentBuffer(*entityArchetype, entityRecord);
			RemoveEntityFromArchetype(entityID, *entityArchetype);
		}
		
		m_EntityArchetypes.erase(entityID);
	}

	bool Scene::DoesEntityExist(EntityID entityID)
	{
		return (m_EntityArchetypes.find(entityID) != m_EntityArchetypes.end());
	}

	size_t Scene::Count() const
	{
		return m_EntityArchetypes.size();
	}

	void Scene::RemoveAll()
	{
		for (auto it = m_EntityArchetypes.begin(); it != m_EntityArchetypes.end(); )
		{
			RemoveEntity(it++->first);
		}

		m_EntityArchetypes.clear();
	}


	// ======== ARCHETYPES ========

	Archetype* Scene::CreateArchetype(const ArchetypeID& archetypeID)
	{
		Archetype* archetype = new Archetype();
		archetype->typeID = archetypeID;

		m_Archetypes.emplace_back(archetype);

		for (int i = 0; i < archetypeID.size(); i++)
		{
			archetype->compData.push_back(nullptr);
			archetype->compDataSize.push_back(0);
		}

		return archetype;
	}

	Archetype* Scene::GetArchetype(const ArchetypeID& archetypeID)
	{
		for (auto* archetype : m_Archetypes)
		{
			if (archetype->typeID == archetypeID)
				return archetype;
		}

		return nullptr;
	}

	// ======== HELPER FUNCTIONS ========

	void Scene::RemoveEntityFromArchetype(EntityID entityID, Archetype& archetype)
	{
		int decrementStartingIndex = -1;
		for (int i = 0; i < archetype.entityIDs.size(); i++)
		{
			if (decrementStartingIndex == -1 && archetype.entityIDs[i] == entityID)
				decrementStartingIndex = i;

			if (decrementStartingIndex != -1)
			{
				EntityRecord& record = m_EntityArchetypes[archetype.entityIDs[i]];
				record.index -= 1;
			}
		}
		if (decrementStartingIndex != -1)
			archetype.entityIDs.erase(archetype.entityIDs.begin() + decrementStartingIndex);
	}

	void Scene::ShrinkComponentBuffer(Archetype& archetype, const EntityRecord& entityRecord)
	{
		for (int typeIndex = 0; typeIndex < archetype.typeID.size(); typeIndex++)
		{
			const ComponentData* compData = m_ComponentMap.at(archetype.typeID[typeIndex]);

			unsigned char* newData;
			const size_t compDataSize = compData->size;
			const size_t targetSize = (archetype.entityIDs.size() - 1) * compDataSize;

			if (targetSize > 0)
			{
#ifdef DEBUG
				//INFO("Archetype component buffer is too large. Shrinking buffer memory ({} -> {}) for ({})...",
				//	archetype.compDataSize[typeIndex],
				//	targetSize, archetype.debugName);
#endif
				newData = new unsigned char[targetSize];
				archetype.compDataSize[typeIndex] = targetSize;

				int offsetIndex = 0;
				for (int entIndex = 0; entIndex < archetype.entityIDs.size(); entIndex++)
				{
					if (entIndex == entityRecord.index)
						continue;

					compData->MoveData(&archetype.compData[typeIndex][entIndex * compDataSize], &newData[offsetIndex * compDataSize]);
					offsetIndex += 1;
				}
			}
			else
			{
#ifdef DEBUG
				//INFO("Deallocating component buffer ({} -> {}) for ({})...",
				//	archetype.compDataSize[typeIndex],
				//	0, archetype.debugName);
#endif
				newData = nullptr;
				archetype.compDataSize[typeIndex] = 0;
			}

			delete[] archetype.compData[typeIndex];
			archetype.compData[typeIndex] = newData;
		}
	}

	//TODO: These names are really bad. newArchetype should not include "NEW" since it's just moving data.
	// INFO log message is also super vague. It's not allocating more memory in some cases.
	void Scene::ResizeAndReallocateComponentBuffer(Archetype& archetype, const ComponentData& compData, int compIndex, size_t targetSize)
	{
#ifdef DEBUG
		//INFO("Archetype component buffer is too small. Allocating more memory ({} -> {}) for ({})...",
		//	archetype.compDataSize[compIndex],
		//	targetSize, archetype.debugName);
#endif
		size_t newCompDataSize = compData.size;
		archetype.compDataSize[compIndex] = targetSize;
		unsigned char* newData = new unsigned char[archetype.compDataSize[compIndex]];
		//If this archetype has entities, we need to loop through all entities and move their data from the old (smaller) to the new (larger) buffer
		for (int entIndex = 0; entIndex < archetype.entityIDs.size(); entIndex++)
			compData.MoveData(&archetype.compData[compIndex][entIndex * newCompDataSize], &newData[entIndex * newCompDataSize]);

		//The data has been moved so we can delete the old buffer and replace it with the new one.
		delete[] archetype.compData[compIndex];
		archetype.compData[compIndex] = newData;
	}
}