#pragma once
#include "entt/entt.hpp"
#include "Entity.h"

namespace LAG
{
	namespace Reflection
	{
		enum class WidgetModes;
	}


	struct ComponentData
	{
		uint32_t ID;
		std::string displayName;
	};

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
		bool Loop(std::function<void(Entity entity, Components&...)> func)
		{
			auto view = m_Registry.view<Components...>();
			if (view.back() == entt::null)
				return false;

			for (auto& current : m_Registry.view<Components...>())
			{
				Entity ent(current, m_Registry);
				func(ent, m_Registry.get<Components>(current)...);
			}

			return true;
		}

		//Loops over all components. When looping, the function pointer is called for every registered component type.
		//When calling the function pointer, an object of type ComponentData is passed in. This object contains info about the component like name and ID. 
		void ComponentLoop(std::function<void(ComponentData& compData)> func);

		void HandleComponentWidgets(Entity* entity, Reflection::WidgetModes mode);


	private:
		bool ReflectComponent(entt::meta_type& compMeta, entt::sparse_set& storageSet, Entity* entity, Reflection::WidgetModes mode);
		void ReflectProperty(entt::meta_data& propData, entt::meta_any& propValues, Entity* entity, Reflection::WidgetModes mode);
		void ReflectType(entt::meta_any& typeValues, Entity* entity, const std::string& propName, Reflection::WidgetModes mode);

		entt::registry m_Registry;
	};
}
