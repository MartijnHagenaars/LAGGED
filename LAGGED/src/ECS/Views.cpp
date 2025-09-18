#include "Views.h"
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
		return ArchetypeView::ComponentRange(m_Scene, m_Archetype.typeID);
	}

	ArchetypeView::ComponentRange::ComponentRange(Scene& scene, CompIdContainer& idContainer) :
		m_Scene(scene), m_IdContainer(idContainer)
	{
	}


	//////////////////////////////////////////////
	// Component Range Iterator implementations //
	//////////////////////////////////////////////

	ArchetypeView::ComponentRange::Iterator::Iterator(Scene& scene, InnerIterator it) :
		m_Scene(scene), m_Ptr(it)
	{
	}

	ComponentView ArchetypeView::ComponentRange::Iterator::operator*() const { return ComponentView(m_Scene, *m_Ptr, *Scene::s_ComponentMap[*m_Ptr]); }
	ComponentView ArchetypeView::ComponentRange::Iterator::operator->() const { return ComponentView(m_Scene, *m_Ptr, *Scene::s_ComponentMap[*m_Ptr]); }

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
		m_Scene(scene), m_ID(id), m_ComponentData(compData), m_ReflectionData(Scene::s_ReflectionMap[id])
	{
	}

	ComponentView::MemberRange ComponentView::Members()
	{
		return ComponentView::MemberRange(m_ReflectionData.members);
	}

	void* ComponentView::GetVoid(EntityID id)
	{
		if (id == ENTITY_NULL)
			return nullptr;

		//TODO: Considering this is the only situation where we access the Scene, do we really need to store a reference to the scene and not just to the uhh
		Scene::EntityRecord& record = m_Scene.m_EntityArchetypes.at(id);
		size_t index = record.archetype->systemCompIndices.at(m_ID);

		return reinterpret_cast<void*>(&record.archetype->compData[index][record.index * m_ComponentData.size]);
	}

	std::any ComponentView::ToAny(void* data)
	{
		return m_ComponentData.VoidToAny(data);
	}

	ComponentView::MemberRange::MemberRange(DataContainer& dataContainer) :
		m_Container(dataContainer)
	{
	}

	ComponentView::MemberRange::Iterator::Iterator(InnerIterator it) : 
		m_Ptr(it)
	{
	}

	MemberView ComponentView::MemberRange::Iterator::operator*() const { return MemberView(*m_Ptr); }
	MemberView ComponentView::MemberRange::Iterator::operator->() const { return MemberView(*m_Ptr); }

	ComponentView::MemberRange::Iterator& ComponentView::MemberRange::Iterator::operator++()
	{
		++m_Ptr;
		return *this;
	}

	ComponentView::MemberRange::Iterator& ComponentView::MemberRange::Iterator::operator--()
	{
		--m_Ptr;
		return *this;
	}

	ComponentView::MemberRange::Iterator ComponentView::MemberRange::Iterator::operator++(int)
	{
		Iterator tempIt = *this;
		++(*this);
		return tempIt;
	}

	ComponentView::MemberRange::Iterator ComponentView::MemberRange::Iterator::operator--(int)
	{
		Iterator tempIt = *this;
		--(*this);
		return tempIt;
	}


	////////////////////////////////
	// MemberView implementations //
	////////////////////////////////

	MemberView::MemberView(ReflectionData::MemberData& memberData) : 
		m_MemberData(memberData)
	{
	}
}