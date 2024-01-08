#pragma once
#include "entt/entt.hpp"
#include "Entity.h"
#include <memory>

namespace LAG
{
	class Scene
	{
	public: 
		Scene();
		~Scene();

		Entity AddEntity();
		void RemoveEntity(uint32_t entityID);

		bool DoesEntityExist(uint32_t entityID); 

		Entity GetEntity(uint32_t entityID);
		size_t Count() const;

		void RemoveAll();
		
		template<typename... Components>
		bool Loop(std::function<void(uint32_t entityID, Components&...)> func)
		{
			auto view = m_Registry.view<Components...>();
			if (view.back() == entt::null)
				return false;

			for (auto& current : m_Registry.view<Components...>())
				func(uint32_t(current), m_Registry.get<Components>(current)...);

			return true;
		}

	private:
		entt::registry m_Registry;

	};
}
