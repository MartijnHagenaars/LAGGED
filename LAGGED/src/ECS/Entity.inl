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
		const auto& record = m_SceneRef->m_EntityArchetypes.find(m_ID);
		if (record == m_SceneRef->m_EntityArchetypes.end())
			return false;

		Archetype* archetype = record->second.archetype;
		ComponentID compID = Scene::GetComponentID<Comp>();
		return (std::find(archetype->typeID.begin(), archetype->typeID.end(), compID)) != archetype->typeID.end();
	}

	template<typename Comp>
	inline Comp* Entity::GetComponent()
	{
		//TODO: Implement function...
		return nullptr;
	}
}