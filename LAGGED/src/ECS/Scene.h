#pragma once
#include "entt/entt.hpp"
#include "Entity/EntityBase.h"

namespace LAG
{
	class Scene
	{
	public: 
		Scene();
		~Scene();

		EntityBase AddEntity()
		{
			entt::entity newEntity = m_Registry.create();
			return EntityBase(newEntity);
		}

	private:
		entt::registry m_Registry;

	};
}
