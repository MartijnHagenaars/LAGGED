#include "Entity.h"

namespace LAG
{
	Entity::Entity(entt::entity entityID, entt::registry& registryPtr) :
		m_EntityID(entityID), m_RegistryPtr(&registryPtr)
	{

	}

	Entity::~Entity()
	{
	}
}