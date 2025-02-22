#pragma once
#include <functional>

#include "Archetype.h"
#include "Utility/Logger.h"

namespace LAG
{
	namespace Reflection
	{
		enum class WidgetModes;
	}

	class Entity;
	class Scene
	{
	public: 
		Scene();
		~Scene();

		Entity AddEntity();
		Entity AddEntity(const std::string& entityName);

		void RemoveEntity(EntityID entityID);
		Entity* DuplicateEntity(EntityID entityID);

		template<typename Comp, typename ...Args>
		Comp* AddComponent(const EntityID entityID, Args&&... args);

		bool DoesEntityExist(EntityID entityID);

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
		template<typename Comp>
		static const ComponentID GetComponentID();

	private:
		static inline EntityID s_EntityCounter = 0;
		static inline ComponentID s_ComponentCounter = 0;

		struct Record
		{
			uint64_t index;
			Archetype* archetype;
		};
		// Track which entity uses which archetype.
		std::unordered_map<EntityID, Record> m_EntityArchetypes;

		struct ComponentData
		{
			uint64_t size;
			std::function<void(unsigned char* src, unsigned char* dest)> moveData;
		};
		std::unordered_map<ComponentID, ComponentData*> m_ComponentMap;

		template<typename Comp>
		ComponentData* RegisterComponent();
	};
}

// Implement the Scene template functions
#include "Scene.inl"