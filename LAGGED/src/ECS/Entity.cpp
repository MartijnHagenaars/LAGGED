#include "Entity.h"
#include "Scene.h"

namespace LAG
{
	Entity::Entity(EntityID entityID, Scene& sceneRef) :
		m_ID(entityID), m_SceneRef(&sceneRef)
	{
	}

	bool Entity::IsValid()
	{
		//TODO: Implement function...
		return false;
	}

	EntityID Entity::GetEntityID() const
	{
		//TODO: Implement function...
		return 0;
	}
}