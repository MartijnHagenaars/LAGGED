#pragma once
#include "entt/entt.hpp"

#include "Entity.h"
#include "Component.h"

namespace LAG
{
	namespace Reflection
	{
		enum class WidgetModes;
	}

	class Scene
	{
	public: 
		Scene();
		~Scene();

		Entity AddEntity();
		Entity AddEntity(const std::string& entityName);

		Entity DuplicateEntity(uint32_t entityID);

		void RemoveEntity(uint32_t entityID);

		bool DoesEntityExist(uint32_t entityID); 

		Entity GetEntity(uint32_t entityID);
		size_t Count() const;

		void RemoveAll();

		/// <summary>
		/// This function is used to loop over all entities in the scene.
		/// </summary>
		/// <typeparam name="...Components">The component types that an entity needs to contain.</typeparam>
		/// <param name="func">The function that is called when looping over an entity that contains all required components.</param>
		/// <returns>Returns true if any entities are found containing all required components..</returns>
		template<typename... Components>
		bool Loop(std::function<void(Entity entity, Components&...)> func)
		{
			//TODO: Implement function...
			return false;
		}

		/// <summary>
		/// Loops over all components. When looping, the function pointer is called for every registered component type.
		/// </summary>
		/// <param name="func">The function that is called when looping over a registered component. When calling the function pointer, an object of type ComponentData is passed in. This object contains info about the component like name and ID.</param>
		void ComponentLoop(std::function<void(Component& comp)> func);

		void HandleComponentWidgets(Entity* entity, Reflection::WidgetModes mode);


	private:
		entt::registry m_Registry;
	};
}
