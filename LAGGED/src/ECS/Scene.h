#pragma once
#include "entt/entt.hpp"
#include "Entity/Entity.h"
#include <memory>

namespace LAG
{
	class Scene
	{
	public: 
		Scene();
		~Scene();

		LAG_API Entity AddEntity()
		{
			entt::entity newEntity = m_Registry.create();
			return Entity(newEntity, m_Registry);
		}

	private:
		entt::registry m_Registry;

	};
}
