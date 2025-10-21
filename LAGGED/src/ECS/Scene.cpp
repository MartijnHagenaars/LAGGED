#include "Scene.h"

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

	EntityID Scene::AddEntity()
	{
		EntityID newId = ++s_EntityCounter;

		auto entArchetype = m_EntityArchetypes.find(newId);
		if (entArchetype != m_EntityArchetypes.end())
			ERROR("Entity with ID {} already exists in EnityArchetypes map.", newId);

		EntityRecord rec = {};
		rec.index = 0;
		rec.archetype = nullptr;
		m_EntityArchetypes.insert({ newId, rec });

		return newId;
	}

	EntityID Scene::AddEntity(const std::string& name)
	{
		EntityID entID = AddEntity();
		AddComponent<DefaultComponent>(entID)->name = name;
		
		return entID;
	}

	EntityID Scene::DuplicateEntity(EntityID id)
	{
		//TODO: Implement function...
		return EntityID();
	}

	void Scene::RemoveEntity(EntityID id)
	{
		const auto& entityRecordIt = m_EntityArchetypes.find(id);
		if (entityRecordIt == m_EntityArchetypes.end())
		{
			ERROR("Tried to delete an entity with ID {}, which does not exist.", id);
			return;
		}
		EntityRecord& entityRecord = entityRecordIt->second;
		Archetype* archetype = entityRecord.archetype;

		if (archetype)
		{
			size_t entityIndex = -1;
			for (size_t i = archetype->entityIDs.size() - 1; i >= 0; i--)
			{
				if (archetype->entityIDs[i] == id)
				{
					entityIndex = i;
					break;
				}
			}

			if (entityIndex != -1)
			{
				size_t endIndex = archetype->entityIDs.size() - 1;
				EntityID ogIdCopy = archetype->entityIDs[entityIndex];

				EntityRecord& record = m_EntityArchetypes[archetype->entityIDs[entityIndex]];
				record.index = entityIndex;

				archetype->entityIDs[entityIndex] = archetype->entityIDs[endIndex];
				archetype->entityIDs[endIndex] = ogIdCopy;
				m_EntityArchetypes[archetype->entityIDs[entityIndex]].index = entityIndex;

				for (int i = 0; i < archetype->typeID.size(); i++)
				{
					TypeID typeID = archetype->typeID[i];
					TypeInfo& compData = GetTypeInfo().at(typeID);

					compData.DestructData(&archetype->compData[i][entityRecord.index * compData.size]);
					compData.MoveData(
						&archetype->compData[i][endIndex * compData.size],
						&archetype->compData[i][entityRecord.index * compData.size]);
				}
				archetype->entityIDs.erase(archetype->entityIDs.begin() + endIndex);

			}
		}

		m_EntityArchetypes.erase(id);
	}

	bool Scene::IsValid(EntityID id)
	{
		return (m_EntityArchetypes.find(id) != m_EntityArchetypes.end());
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

	Scene::ArchetypeRange Scene::Range()
	{
		return ArchetypeRange(*this, m_Archetypes);
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
			archetype->systemCompIndices.emplace(archetypeID[i], i);
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

	void Scene::RemoveEntityFromArchetype(EntityID id, Archetype& archetype)
	{
		size_t entityIndex = -1;
		for (size_t i = archetype.entityIDs.size() - 1; i >= 0; i--)
		{
			if (archetype.entityIDs[i] == id)
			{
				entityIndex = i;
				break;
			}
		}

		if (entityIndex != -1)
		{
			size_t endIndex = archetype.entityIDs.size() - 1;
			size_t ogIdCopy = archetype.entityIDs[entityIndex];

			EntityRecord& record = m_EntityArchetypes[archetype.entityIDs[entityIndex]];
			record.index = entityIndex;

			archetype.entityIDs[entityIndex] = archetype.entityIDs[endIndex];
			archetype.entityIDs[endIndex] = ogIdCopy;
			archetype.entityIDs.erase(archetype.entityIDs.begin() + endIndex);

		}

	}

	void Scene::ShrinkComponentBuffer(Archetype& archetype, const EntityRecord& entityRecord)
	{
		for (int typeIndex = 0; typeIndex < archetype.typeID.size(); typeIndex++)
		{
			const TypeInfo& compData = GetTypeInfo().at(archetype.typeID[typeIndex]);

			unsigned char* newData;
			const size_t compDataSize = compData.size;
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

					compData.MoveData(&archetype.compData[typeIndex][entIndex * compDataSize], &newData[offsetIndex * compDataSize]);
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
	void Scene::ResizeAndReallocateComponentBuffer(Archetype& archetype, const TypeInfo& compData, int compIndex, size_t targetSize)
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


	/////////////////////////////
	// ITERATOR IMPLEMENTATION //
	/////////////////////////////

	Scene::ArchetypeRange::ArchetypeRange(Scene& scene, ArchContainer& container) :
		m_Scene(scene), m_Container(container)
	{}
	Scene::ArchetypeRange::Iterator Scene::ArchetypeRange::begin() const { return ArchetypeRange::Iterator(m_Scene, m_Container.begin()); }
	Scene::ArchetypeRange::Iterator Scene::ArchetypeRange::end() const { return Iterator(m_Scene, m_Container.end()); }


	Scene::ArchetypeRange::Iterator::Iterator(Scene& scene, InnerIterator it)
		: m_Scene(scene), m_Ptr(it) 
	{}

	ArchetypeView Scene::ArchetypeRange::Iterator::operator*() const { return ArchetypeView(m_Scene, **m_Ptr); }
	ArchetypeView Scene::ArchetypeRange::Iterator::operator->() const { return ArchetypeView(m_Scene, **m_Ptr); }

	Scene::ArchetypeRange::Iterator& Scene::ArchetypeRange::Iterator::operator++()
	{
		++m_Ptr;
		return *this;
	}
	Scene::ArchetypeRange::Iterator& Scene::ArchetypeRange::Iterator::operator--()
	{
		--m_Ptr;
		return *this;
	}

	Scene::ArchetypeRange::Iterator Scene::ArchetypeRange::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++(*this);
		return temp;
	}

	Scene::ArchetypeRange::Iterator Scene::ArchetypeRange::Iterator::operator--(int)
	{
		Iterator temp = *this;
		--(*this);
		return temp;
	}
}