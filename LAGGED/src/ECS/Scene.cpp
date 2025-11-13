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
		AddComponent<EditorComponent>(entID)->name = name;

		return entID;
	}

	EntityID Scene::DuplicateEntity(EntityID id)
	{
		// TODO: Implement function...
		return ENTITY_NULL;
	}

	void* Scene::AddComponent(EntityID entityID, TypeID compID)
	{
		TypeInfo* typeInfo = nullptr;
		if (auto typeInfoIt = GetTypeInfo().find(compID); typeInfoIt != GetTypeInfo().end())
			typeInfo = &typeInfoIt->second;
		else
			CRITICAL("Failed to add component to entity {}: Component with typeID {} is not registered.", entityID, compID);

		const auto& entityRecordIt = m_EntityArchetypes.find(entityID);
		if (entityRecordIt == m_EntityArchetypes.end())
			CRITICAL("Failed to find Entity with ID {} in EntityArchetype lookup map.", entityID);

		EntityRecord& entityRec = entityRecordIt->second;
		Archetype* oldArchetype = entityRec.archetype;
		Archetype* newArchetype = nullptr;
		void* newCompPtr = nullptr;

		// When oldArchetype is equal to nullptr, the entity doesn't hold any component data.
		// If oldArchetype is NOT equal to nullptr, the entity already holds onto some component data.
		if (oldArchetype == nullptr)
		{
			// This is the first time we're adding a component to this entity. 
			// Create a new archetypeID which contains this component ID
			ArchetypeID newArchetypeID = { compID };

			newArchetype = GetArchetype(newArchetypeID);
			if (newArchetype == nullptr)
			{
				newArchetype = CreateArchetype(newArchetypeID);
#ifdef DEBUG
				INFO("Creating Archetype with {} component : ({})",
					newArchetype->typeID.size(), newArchetype->debugName
				);
#endif
			}

			size_t compSize = typeInfo->size;
			size_t targetSize = (newArchetype->entityIDs.size() * compSize) + compSize;
			if (newArchetype->compDataSize[0] <= targetSize)
			{
				size_t newSize = (newArchetype->compDataSize[0] * ARCHETYPE_ALLOC_GROWTH) + compSize;
				ResizeAndReallocateComponentBuffer(*newArchetype, *typeInfo, 0, newSize);
			}

			// Construct a new Comp object (with the correct Args) and place it into the component data array
			size_t compDataIndex = newArchetype->entityIDs.size() * compSize;
			newCompPtr = &newArchetype->compData[0][compDataIndex];
			typeInfo->Construct(static_cast<unsigned char*>(newCompPtr));
		}
		else
		{
			// Check if this component already exists on the entity.
			// If that's the case, log a warning and return the component
			if (std::find(oldArchetype->typeID.begin(), oldArchetype->typeID.end(), compID) != oldArchetype->typeID.end())
			{
#ifdef DEBUG
				CRITICAL("Component ({}) already exists on entity with ID {}", typeInfo->debugName, compID);
#endif
				return nullptr; // TODO: Return a valid pointer.
			}

			ArchetypeID newArchetypeID = oldArchetype->typeID;
			newArchetypeID.push_back(compID);
			std::sort(newArchetypeID.begin(), newArchetypeID.end());

			newArchetype = GetArchetype(newArchetypeID);
			if (newArchetype == nullptr)
			{
				newArchetype = CreateArchetype(newArchetypeID);
#ifdef DEBUG
				INFO("Creating Archetype with {} components: {}",
					newArchetype->typeID.size(), newArchetype->debugName);
#endif
			}

			// This for-loop goes over all components.
			// It handles allocating more space in the new archetype and moving the component data from the old archetype to the new one. 
			// In the end, after everything has been moved, the new component is added. 
			for (int compIndex = 0; compIndex < newArchetypeID.size(); compIndex++)
			{
				const TypeID typeID = newArchetypeID[compIndex];
				TypeInfo& newCompInfo = GetTypeInfo().at(typeID);

				size_t newCompSize = newCompInfo.size;
				size_t currentSize = newArchetype->entityIDs.size() * newCompSize;
				size_t newSize = currentSize + newCompSize;

				// If there is no more space left for a component in a archetype, allocate some more and move all the previous 
				// component data from the old archetype buffer to the new one. 
				if (newSize > newArchetype->compDataSize[compIndex])
				{
					size_t targetSize = (newArchetype->compDataSize[compIndex] * ARCHETYPE_ALLOC_GROWTH) + newCompSize;
					ResizeAndReallocateComponentBuffer(*newArchetype, newCompInfo, compIndex, targetSize);
				}


				// In this loop, we move all the data from the old archetype to the new one. 
				// While doing this, the new component is also added to the new archetype at the correct index.
				for (int i = 0; i < oldArchetype->typeID.size(); i++)
				{
					TypeID oldTypeID = oldArchetype->typeID[i];
					bool matchingIDs = (oldTypeID == typeID);
					if (matchingIDs)
					{
						TypeInfo& oldCompInfo = GetTypeInfo().at(oldArchetype->typeID[i]);
						oldCompInfo.Move(&oldArchetype->compData[i][entityRec.index * oldCompInfo.size], &newArchetype->compData[compIndex][currentSize]);
						break;
					}
					else if (i == (oldArchetype->typeID.size() - 1) && !matchingIDs)
					{
						newCompPtr = &newArchetype->compData[compIndex][currentSize];
						typeInfo->Construct(static_cast<unsigned char*>(newCompPtr));
					}
				}
			}

			RemoveEntityFromArchetype(entityID, *oldArchetype);
		}

		// Update the entity archetype record 
		// and add the EntityID to the archetype. 
		newArchetype->entityIDs.push_back(entityID);

		entityRec.archetype = newArchetype;
		entityRec.index = newArchetype->entityIDs.size() - 1;
		return newCompPtr;
	}

	void Scene::RemoveComponent(EntityID entityID, TypeID compID)
	{
		TypeInfo* typeInfo = nullptr;
		if (auto typeInfoIt = GetTypeInfo().find(compID); typeInfoIt != GetTypeInfo().end())
			typeInfo = &typeInfoIt->second;
		else
			CRITICAL("Failed to remove component from entity {}: Component with typeID {} is not registered.", entityID, compID);

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
#ifdef DEBUG
			WARNING("Failed to remove component: Entity ({}) does not contain component {} ({}).", entityID, typeInfo->debugName, compID);
#endif
			return;
		}

		// Remove the old TypeID and construct the new ArchetypeID
		ArchetypeID newArchetypeID = oldArchetype->typeID;
		newArchetypeID.erase(newArchetypeID.begin() + (archetypeIdIt - oldArchetype->typeID.begin()));
		std::sort(newArchetypeID.begin(), newArchetypeID.end());

		Archetype* newArchetype = nullptr;
		if (!newArchetypeID.empty())
		{
			newArchetype = GetArchetype(newArchetypeID);
			if (newArchetype == nullptr)
			{
				newArchetype = CreateArchetype(newArchetypeID);
#ifdef DEBUG
				INFO("Creating archetype with {} components: {}",
					newArchetype->typeID.size(), newArchetype->debugName);
#endif
			}
		}

		for (int i = 0; i < oldArchetype->typeID.size(); i++)
		{
			// FIXME: Could be fast enough... Profile without noise generation
			//TypeID currTypeID = oldArchetype->typeID[i];
			TypeID oldArchTypeID = oldArchetype->typeID[i];
			if (oldArchTypeID == compID)
			{
				const TypeInfo& compTypeInfo = GetTypeInfo().at(oldArchTypeID);
				compTypeInfo.Destruct(&oldArchetype->compData[i][entityRecord.index * compTypeInfo.size]);
			}
			else
			{
				size_t newArchTypeIndex = newArchetype->systemCompIndices.at(oldArchTypeID);
				TypeID newArchTypeID = newArchetype->typeID[newArchTypeIndex];
				if (oldArchTypeID == newArchTypeID)
				{
					const TypeInfo& compTypeInfo = GetTypeInfo().at(newArchTypeID);
					size_t newCompDataSize = compTypeInfo.size;
					size_t currentSize = newArchetype->entityIDs.size() * newCompDataSize;
					size_t nextSize = currentSize + newCompDataSize;
					if (nextSize > newArchetype->compDataSize[newArchTypeIndex])
					{
						size_t targetSize = (newArchetype->compDataSize[newArchTypeIndex] * ARCHETYPE_ALLOC_GROWTH) + newCompDataSize;
						ResizeAndReallocateComponentBuffer(*newArchetype, compTypeInfo, newArchTypeIndex, targetSize);
					}

					compTypeInfo.Move(&oldArchetype->compData[i][entityRecord.index * compTypeInfo.size], &newArchetype->compData[newArchTypeIndex][currentSize]);
				}
			}
		}

		if (newArchetype)
			newArchetype->entityIDs.push_back(entityID);

		if (!oldArchetype->entityIDs.empty())
		{
			RemoveEntityFromArchetype(entityID, *oldArchetype);
			//ShrinkComponentBuffer(*oldArchetype, entityRecord);
		}

		if (newArchetype)
		{
			entityRecord.index = newArchetype->entityIDs.size() - 1;
			entityRecord.archetype = newArchetype;
		}
		else
		{
			entityRecord.index = 0;
			entityRecord.archetype = nullptr;
		}


	}

	bool Scene::HasComponent(EntityID entityID, TypeID compID)
	{
		const auto& recordIt = m_EntityArchetypes.find(entityID);
		if (recordIt == m_EntityArchetypes.end())
			return false;

		Archetype* archetype = recordIt->second.archetype;
		return (std::find(archetype->typeID.begin(), archetype->typeID.end(), compID)) != archetype->typeID.end();
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

					compData.Destruct(&archetype->compData[i][entityRecord.index * compData.size]);
					compData.Move(
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
		return ArchetypeRange(m_Archetypes);
	}

	std::vector<TypeID> Scene::QueryCompIDs()
	{
		std::vector<TypeID> compIDs;
		compIDs.reserve(s_ReflectedCompInfo.size());
		for (const auto& it : s_ReflectedCompInfo)
			compIDs.emplace_back(it.first);

		return compIDs;
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

#ifdef DEBUG
		std::string debugName = GetTypeInfo().at(archetypeID[0]).debugName;
		for (int i = 1; i < archetypeID.size(); i++)
			debugName += "|" + GetTypeInfo().at(archetypeID[i]).debugName;
		archetype->debugName = debugName;
#endif

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

			EntityRecord& record = m_EntityArchetypes[archetype.entityIDs[entityIndex]];
			EntityRecord& swappedRec = m_EntityArchetypes.at(archetype.entityIDs[endIndex]);
			for (int i = 0; i < archetype.typeID.size(); i++)
			{
				TypeInfo& typeInfo = GetTypeInfo()[archetype.typeID[i]];
				typeInfo.Move(&archetype.compData[i][swappedRec.index * typeInfo.size], &archetype.compData[i][record.index * typeInfo.size]);
			}

			swappedRec.index = entityIndex;
			archetype.entityIDs[entityIndex] = archetype.entityIDs[endIndex];
			archetype.entityIDs.erase(archetype.entityIDs.begin() + endIndex);
		}

	}

	void Scene::ShrinkComponentBuffer(Archetype& archetype, const EntityRecord& entityRecord)
	{
		for (int typeIndex = 0; typeIndex < archetype.typeID.size(); typeIndex++)
		{
			if (archetype.entityIDs.empty())
			{
				delete[] archetype.compData[typeIndex];
				archetype.compData[typeIndex] = nullptr;
				archetype.compDataSize[typeIndex] = 0;
				return;
			}

			const TypeInfo& compData = GetTypeInfo().at(archetype.typeID[typeIndex]);

			unsigned char* newData;
			const size_t compDataSize = compData.size;
			const size_t targetSize = (archetype.entityIDs.size() - 1) * compDataSize;

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

				compData.Move(&archetype.compData[typeIndex][entIndex * compDataSize], &newData[offsetIndex * compDataSize]);
				offsetIndex += 1;
			}

			delete[] archetype.compData[typeIndex];
			archetype.compData[typeIndex] = newData;
		}
	}

	void Scene::ResizeAndReallocateComponentBuffer(Archetype& archetype, const TypeInfo& typeInfo, int compIndex, size_t targetSize)
	{
#ifdef DEBUG
		//INFO("Archetype component buffer is too small. Allocating more memory ({} -> {}) for ({})...",
		//	archetype.compDataSize[compIndex],
		//	targetSize, archetype.debugName);
#endif
		size_t newCompDataSize = typeInfo.size;
		archetype.compDataSize[compIndex] = targetSize;
		unsigned char* newData = new unsigned char[archetype.compDataSize[compIndex]];
		//If this archetype has entities, we need to loop through all entities and move their data from the old (smaller) to the new (larger) buffer
		for (int entIndex = 0; entIndex < archetype.entityIDs.size(); entIndex++)
			typeInfo.Move(&archetype.compData[compIndex][entIndex * newCompDataSize], &newData[entIndex * newCompDataSize]);

		//The data has been moved so we can delete the old buffer and replace it with the new one.
		delete[] archetype.compData[compIndex];
		archetype.compData[compIndex] = newData;
	}


	/////////////////////////////
	// ITERATOR IMPLEMENTATION //
	/////////////////////////////

	Scene::ArchetypeRange::ArchetypeRange(ArchContainer& container) :
		m_Container(container)
	{
	}

	Scene::ArchetypeRange::Iterator Scene::ArchetypeRange::begin() const { return ArchetypeRange::Iterator(m_Container.begin()); }
	Scene::ArchetypeRange::Iterator Scene::ArchetypeRange::end() const { return ArchetypeRange::Iterator(m_Container.end()); }


	Scene::ArchetypeRange::Iterator::Iterator(InnerIterator it)
		: m_Ptr(it)
	{
	}

	ArchetypeView Scene::ArchetypeRange::Iterator::operator*() const { return ArchetypeView(**m_Ptr); }
	ArchetypeView Scene::ArchetypeRange::Iterator::operator->() const { return ArchetypeView(**m_Ptr); }

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