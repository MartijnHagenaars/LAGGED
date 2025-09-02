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
	inline bool Entity::HasComponent() const
	{
		return m_SceneRef->HasComponent<Comp>(m_ID);
	}

	template<typename Comp>
	inline Comp* Entity::GetComponent() const
	{
		return m_SceneRef->GetComponent<Comp>(m_ID);
	}
}