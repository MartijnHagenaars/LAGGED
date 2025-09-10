#include "Containers.h"
#include "Scene.h"

namespace LAG
{
	ArchetypeView::ArchetypeView(Scene& scene, Archetype& archetype) : 
		m_Scene(scene), m_Archetype(archetype)
	{
	}

	bool ArchetypeView::Contains(EntityID id) const 
	{
		for (const EntityID& entIt : m_Archetype.entityIDs)
		{
			if (entIt == id)
				return true;
		}
		return false;
	}

	ArchetypeView::ComponentRange ArchetypeView::Types()
	{
		return ArchetypeView::ComponentRange(m_Scene, m_Archetype.typeID, m_Scene.m_ComponentMap);
	}

	ArchetypeView::ComponentRange::ComponentRange(Scene& scene, CompIdContainer& idContainer, CompDataContainer& dataContainer) :
		m_Scene(scene), m_IdContainer(idContainer), m_DataContainer(dataContainer)
	{
	}


	//////////////////////////////////////////////
	// Component Range Iterator implementations //
	//////////////////////////////////////////////

	ArchetypeView::ComponentRange::Iterator::Iterator(Scene& scene, InnerIterator it, CompDataContainer& compData) :
		m_Scene(scene), m_Ptr(it), m_ComponentData(compData)
	{}

	ComponentView ArchetypeView::ComponentRange::Iterator::operator*() const { return ComponentView(m_Scene, *m_Ptr, *m_ComponentData[*m_Ptr]); }
	ComponentView ArchetypeView::ComponentRange::Iterator::operator->() const { return ComponentView(m_Scene, *m_Ptr, *m_ComponentData[*m_Ptr]); }

	ArchetypeView::ComponentRange::Iterator& ArchetypeView::ComponentRange::Iterator::operator++()
	{ 
		++m_Ptr; 
		return *this; 
	}

	ArchetypeView::ComponentRange::Iterator& ArchetypeView::ComponentRange::Iterator::operator--()
	{
		--m_Ptr;
		return *this;
	}

	ArchetypeView::ComponentRange::Iterator ArchetypeView::ComponentRange::Iterator::operator++(int)
	{
		Iterator tempIt = *this;
		++(*this);
		return tempIt;
	}

	ArchetypeView::ComponentRange::Iterator ArchetypeView::ComponentRange::Iterator::operator--(int)
	{
		Iterator tempIt = *this;
		--(*this);
		return tempIt;
	}


	///////////////////////////////////
	// ComponentView implementations //
	///////////////////////////////////

	ComponentView::ComponentView(Scene& scene, ComponentID id, ComponentData& compData) : 
		m_Scene(scene), m_ID(id), m_ComponentData(compData)
	{
	}

	void* ComponentView::GetVoid(EntityID id)
	{
		if (id == ENTITY_NULL)
			return nullptr;

		Scene::EntityRecord& record = m_Scene.m_EntityArchetypes.at(id);
		size_t index = record.archetype->systemCompIndices.at(m_ID);

		return reinterpret_cast<void*>(&record.archetype->compData[index][record.index * m_ComponentData.size]);
	}

	std::any ComponentView::ToAny(void* data)
	{
		return m_ComponentData.VoidToAny(data);
	}
}