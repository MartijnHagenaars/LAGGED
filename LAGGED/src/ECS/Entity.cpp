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

	void Entity::AddComponent(uint32_t componentID)
	{
		//m_RegistryPtr->assign(componentID, m_EntityID);
		//m_RegistryPtr->emplace(m_EntityID, componentID);
	}

	uint32_t Entity::GetEntityID() const
	{
		return static_cast<uint32_t>(m_EntityID);
	}
}