#include "Entity.h"

namespace LAG
{
	Entity::Entity(entt::entity entityID, entt::registry& registryPtr) :
		m_EntityID(entityID), m_RegistryPtr(&registryPtr)
	{
	}

	Entity::Entity(const Entity& entity) : 
		m_EntityID(entity.m_EntityID), m_RegistryPtr(entity.m_RegistryPtr)
	{
	}

	bool Entity::IsValid()
	{
		//TODO: Implement function...
		return false;
	}

	uint32_t Entity::GetEntityID() const
	{
		//TODO: Implement function...
		return 0;
	}
}