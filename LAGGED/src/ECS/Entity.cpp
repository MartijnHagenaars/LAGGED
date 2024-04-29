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
		return m_EntityID != entt::tombstone;
	}

	uint32_t Entity::GetEntityID() const
	{
		return static_cast<uint32_t>(m_EntityID);
	}
}