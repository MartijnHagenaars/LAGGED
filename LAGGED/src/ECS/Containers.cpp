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
		return ArchetypeView::ComponentRange(m_Archetype.typeID, m_Scene.m_ComponentMap);
	}

	ArchetypeView::ComponentRange::ComponentRange(CompIdContainer& idContainer, CompDataContainer& dataContainer) :
		m_IdContainer(idContainer), m_DataContainer(dataContainer)
	{
	}

	//////////////////////////////////////////////
	// Component Range Iterator implementations //
	//////////////////////////////////////////////

	ArchetypeView::ComponentRange::Iterator::Iterator(InnerIterator it, CompDataContainer& compData) :
		m_Ptr(it), m_ComponentData(compData)
	{}

	ComponentView ArchetypeView::ComponentRange::Iterator::operator*() const { return ComponentView(*m_ComponentData[*m_Ptr]); }
	ComponentView ArchetypeView::ComponentRange::Iterator::operator->() const { return ComponentView(*m_ComponentData[*m_Ptr]); }

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


	ComponentView::ComponentView(ComponentData& compData) : 
		m_ComponentData(compData)
	{
	}
}