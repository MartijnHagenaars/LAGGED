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

		LAG_API Entity AddEntity();
		LAG_API bool DoesEntityExist(uint32_t entityID); 
		
		template<typename... Components>
		void Loop(std::function<void(uint32_t entityID, Components&...)> func)
		{
			for (auto& current : m_Registry.view<Components...>())
				func(uint32_t(current), m_Registry.get<Components>(current)...);
		}

	private:
		entt::registry m_Registry;

	};
}
