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
		
		template<class Comp>
		void Loop(std::function<void(uint32_t entityID, Comp& comp)> func)
		{
			for (auto& current : m_Registry.view<Comp>())
				func(uint32_t(current), m_Registry.get<Comp>(current));
		}
	private:
		entt::registry m_Registry;

	};
}
