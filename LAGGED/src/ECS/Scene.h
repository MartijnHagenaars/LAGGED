#pragma once
#include <functional>
#include <algorithm>

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
		friend class Entity;
	public: 
		Scene();
		~Scene();

		Entity AddEntity();
		Entity AddEntity(const std::string& entityName);

		void RemoveEntity(EntityID entityID);
		Entity* DuplicateEntity(EntityID entityID);

		template<typename Comp, typename ...Args>
		Comp* AddComponent(const EntityID entityID, Args&&... compArgs);

		bool DoesEntityExist(EntityID entityID);

		size_t Count() const;

		void RemoveAll();

		template<typename ...Comps>
		void RunSystem(std::function<void(EntityID, Comps*...)> func);


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
		Archetype* CreateArchetype(const ArchetypeID& archetypeID);
		Archetype* GetArchetype(const ArchetypeID& archetypeID);

		template<typename Comp>
		static const ComponentID GetComponentID();

		struct ComponentData;
		template<typename Comp>
		ComponentData* RegisterComponent();

	private:
		static inline EntityID s_EntityCounter = 0;
		static inline ComponentID s_ComponentCounter = 0;

		// This vector stores all possible archetypes
		std::vector<Archetype*> m_Archetypes;

		struct ArchetypeRecord
		{
			uint64_t index;
			Archetype* archetype;
		};
		// This map stores all entities and the archetypes that they use.
		std::unordered_map<EntityID, ArchetypeRecord> m_EntityArchetypes;

		struct ComponentData
		{
			uint64_t size;
			std::function<void(unsigned char* src, unsigned char* dest)> moveData;
#ifdef DEBUG
			std::string debugName;
#endif
		};
		// This map links all ComponentIDs to the correct ComponentData struct. 
		// The ComponentData struct contains useful information / functions for managing component data.
		std::unordered_map<ComponentID, ComponentData*> m_ComponentMap;

	};
}

// Implement the Scene template functions
#include "Scene.inl"