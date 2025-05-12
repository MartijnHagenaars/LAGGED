#include "Entity.h"
#include "Scene.h"

namespace LAG
{
	Entity::Entity(Scene& sceneRef, EntityID entityID) :
		m_SceneRef(&sceneRef), m_ID(entityID)
	{
	}

	bool Entity::Valid() const
	{
		return m_ID != ENTITY_NULL;
	}

	EntityID Entity::ID() const
	{
		return m_ID;
	}


	/////////////////////////////
	// ITERATOR IMPLEMENTATION //
	/////////////////////////////

	SceneReflect::ComponentData* LAG::Entity::Iterator::operator*() const
	{
		return &SceneReflect::Get().GetComponentProperties().at(*m_IdPtr);
	}

	SceneReflect::ComponentData* LAG::Entity::Iterator::operator->()
	{
		return &SceneReflect::Get().GetComponentProperties().at(*m_IdPtr);
	}

	Entity::Iterator& Entity::Iterator::operator++()
	{
		m_IdPtr++;
		return *this;
	}

	Entity::Iterator Entity::Iterator::operator++(int)
	{
		Iterator tempIt = *this;
		++(*this);
		return tempIt;
	}

	Entity::Iterator& Entity::Iterator::operator--()
	{
		m_IdPtr--;
		return *this;
	}

	Entity::Iterator Entity::Iterator::operator--(int)
	{
		Iterator tempIt = *this;
		--(*this);
		return tempIt;
	}

	Entity::Iterator Entity::begin()
	{
		ArchetypeID& archetype = m_SceneRef->m_EntityArchetypes.at(m_ID).archetype->typeID;
		return archetype.empty() ? end() : Iterator(&archetype[0]);
	}

	Entity::Iterator Entity::end()
	{
		ArchetypeID& archetype = m_SceneRef->m_EntityArchetypes.at(m_ID).archetype->typeID;
		return archetype.empty() ? end() : Iterator(&archetype[archetype.size() - 1]);
	}
}