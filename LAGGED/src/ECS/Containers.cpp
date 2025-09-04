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
		return ArchetypeView::ComponentRange(m_Scene.m_ComponentMap);
	}

	ArchetypeView::ComponentRange::ComponentRange(CompContainer& container) :
		m_Container(container)
	{
	}

	//////////////////////////////////////////////
	// Component Range Iterator implementations //
	//////////////////////////////////////////////

	ArchetypeView::ComponentRange::Iterator::Iterator(InnerIterator it) :
		m_Ptr(it)
	{}

	ComponentData& ArchetypeView::ComponentRange::Iterator::operator*() const { return *m_Ptr->second; }
	ComponentData* ArchetypeView::ComponentRange::Iterator::operator->() const { return m_Ptr->second; }

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
}