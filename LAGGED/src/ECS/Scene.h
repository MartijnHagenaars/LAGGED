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
		friend class SceneReflect;
	public: 
		Scene();
		~Scene();

		Entity AddEntity();
		Entity AddEntity(const std::string& entityName);

		void RemoveEntity(EntityID entityID);
		Entity DuplicateEntity(EntityID entityID);

		template<typename Comp, typename ...Args>
		Comp* AddComponent(const EntityID entityID, Args&&... compArgs);

		template<typename Comp>
		void RemoveComponent(const EntityID entityID);

		template<typename Comp>
		bool HasComponent(const EntityID& entityID);

		template<typename Comp>
		Comp* GetComponent(const EntityID& entityID);

		/// <summary>
		/// Checks whether an entity with a specific ID exists.
		/// </summary>
		/// <returns>Returns true if an entity with EntityID exists.</returns>
		bool DoesEntityExist(EntityID entityID);

		/// <summary>
		/// Return the number of active entities
		/// </summary>
		size_t Count() const;

		void RemoveAll();

		template<typename ...Comps>
		void RunSystem(std::function<void(EntityID, Comps*...)> func);

		template<typename ...Comps>
		std::vector<EntityID> GetEntitiesWithComponents();

	private:
		struct EntityRecord;
		struct ComponentData;

	public:
		struct Iterator
		{
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;
			using InnerIterator = std::unordered_map<EntityID, EntityRecord>::iterator;

			Iterator(InnerIterator ptr);

			const EntityID& operator*() const;
			const EntityID* operator->() const;

			Iterator& operator++();
			Iterator operator++(int);
			Iterator& operator--();
			Iterator operator--(int);

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_Ptr == b.m_Ptr; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_Ptr != b.m_Ptr; }

		private:
			InnerIterator m_Ptr;
		};

		Iterator begin();
		Iterator end();

	private:
		Archetype* CreateArchetype(const ArchetypeID& archetypeID);
		Archetype* GetArchetype(const ArchetypeID& archetypeID);

		template<typename Comp>
		static const ComponentID GetComponentID();

		template<typename Comp>
		ComponentData* RegisterComponent();

		void RemoveEntityFromArchetype(EntityID entityID, Archetype& archetype);
		void ShrinkComponentBuffer(Archetype& archetype, const EntityRecord& entityRecord);
		void ResizeAndReallocateComponentBuffer(Archetype& archetype, const ComponentData& componentData, int componentIndex, size_t targetSize);

	private:
		static inline EntityID s_EntityCounter = 0;
		static inline ComponentID s_ComponentCounter = 0;
		
		// This vector stores all possible archetypes
		std::vector<Archetype*> m_Archetypes;

		struct EntityRecord
		{
			size_t index;
			Archetype* archetype;
		};
		// This map stores all entities and the archetypes that they use.
		std::unordered_map<EntityID, EntityRecord> m_EntityArchetypes;

		struct ComponentData
		{
			size_t size = -1;

			std::function<void(unsigned char* dest)> CreateObjectInMemory;
			std::function<void(unsigned char* src, unsigned char* dest)> MoveData;
			std::function<void(unsigned char* data)> DestructData;
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