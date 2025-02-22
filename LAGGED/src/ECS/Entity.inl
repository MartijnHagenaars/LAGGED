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
		//TODO: Implement function...
		return false;
	}

	template<typename Comp>
	inline Comp* Entity::GetComponent()
	{
		//TODO: Implement function...
		return nullptr;
	}
}