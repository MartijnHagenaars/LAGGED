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
		//TODO: Implement function...
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
		const auto& recordIt = m_SceneRef->m_EntityArchetypes.find(m_ID);
		if (recordIt == m_SceneRef->m_EntityArchetypes.end() || recordIt->second.archetype == nullptr)
		{
			CRITICAL("Failed to run GetComponent: Record is nullptr.");
			return nullptr;
		}

		int dataOffset = 0;
		const Scene::ArchetypeRecord& record = recordIt->second;
		Archetype* archetype = record.archetype;

		for (int i = 0; i < archetype->typeID.size(); i++)
		{
			if (archetype->typeID[i] == Scene::GetComponentID<Comp>())
				return reinterpret_cast<Comp*>(&archetype->compData[dataOffset][record.index * sizeof(Comp)]);
			else
				++dataOffset;
		}

		INFO("Component ({}) not found on entity {}", typeid(Comp).name(), m_ID);
		printf("INFO: Component not found.");
		return nullptr;
	}
}