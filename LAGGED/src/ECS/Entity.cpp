#include "Entity.h"
#include "Scene.h"

namespace LAG
{
	Entity::Entity(Scene& sceneRef, EntityID entityID) :
		m_SceneRef(&sceneRef), m_ID(entityID)
	{
	}

	bool Entity::IsValid()
	{
		return m_ID != ENTITY_NULL;
	}

	EntityID Entity::GetEntityID() const
	{
		return m_ID;
	}
}