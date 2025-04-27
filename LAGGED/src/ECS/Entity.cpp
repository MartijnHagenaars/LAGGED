#include "Entity.h"
#include "Scene.h"

namespace LAG
{
	Entity::Entity(Scene& sceneRef, EntityID entityID) :
		m_SceneRef(&sceneRef), m_ID(entityID)
	{
	}

	bool Entity::Valid()
	{
		return m_ID != ENTITY_NULL;
	}

	EntityID Entity::ID() const
	{
		return m_ID;
	}
}