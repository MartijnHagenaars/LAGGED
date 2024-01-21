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
		Entity AddEntity(const std::string& entityName);

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

		void DrawComponentWidgets(uint32_t entityID);


	private:
		bool ReflectComponent(entt::meta_type& compMeta, entt::sparse_set& storageSet, uint32_t entityID);
		void ReflectProperty(entt::meta_data& propData, entt::meta_any& propValues, uint32_t entityID);
		void ReflectType(entt::meta_data& typeData, entt::meta_any& typeValues, uint32_t entityID, const std::string& propName);

		entt::registry m_Registry;
	};
}
