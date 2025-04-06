#include "Entity.h"
#pragma once

namespace LAG
{
	template<typename Comp, typename ...Args>
	inline Comp* Entity::AddComponent(Args && ...args)
	{
		return m_SceneRef->AddComponent<Comp>(m_ID, args...);
	}

	template<typename Comp>
	inline void Entity::RemoveComponent()
	{
		m_SceneRef->RemoveComponent<Comp>(m_ID);
	}

	template<typename Comp>
	inline bool Entity::HasComponent()
	{
		const auto& recordIt = m_SceneRef->m_EntityArchetypes.find(m_ID);
		if (recordIt == m_SceneRef->m_EntityArchetypes.end())
			return false;

		Archetype* archetype = recordIt->second.archetype;
		ComponentID compID = Scene::GetComponentID<Comp>();
		return (std::find(archetype->typeID.begin(), archetype->typeID.end(), compID)) != archetype->typeID.end();
	}

	template<typename Comp>
	inline Comp* Entity::GetComponent()
	{
		if (m_ID == ENTITY_NULL)
		{
			CRITICAL("Failed to run GetComponent: EntityID is null.");
			return nullptr;
		}

		const auto& recordIt = m_SceneRef->m_EntityArchetypes.find(m_ID);
		if (recordIt == m_SceneRef->m_EntityArchetypes.end() || recordIt->second.archetype == nullptr)
		{
			CRITICAL("Failed to run GetComponent for Entity {}: Record is null.", m_ID);
			return nullptr;
		}

		int dataOffset = 0;
		const Scene::EntityRecord& record = recordIt->second;
		Archetype* archetype = record.archetype;

		for (int i = 0; i < archetype->typeID.size(); i++)
		{
			if (archetype->typeID[i] == Scene::GetComponentID<Comp>())
				return reinterpret_cast<Comp*>(&archetype->compData[dataOffset][record.index * sizeof(Comp)]);
			else
				++dataOffset;
		}

		return nullptr;
	}
}