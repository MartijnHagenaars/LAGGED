#pragma once
#include <functional>

#include "Entity.h"

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

		Entity* AddEntity();
		Entity* AddEntity(const std::string& entityName);

		Entity* DuplicateEntity(EntityID entityID);

		void RemoveEntity(EntityID entityID);

		bool DoesEntityExist(EntityID entityID);

		Entity* GetEntity(EntityID entityID);
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

	private:
	};
}
