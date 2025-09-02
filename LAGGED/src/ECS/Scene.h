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

		//TODO: Consider removing this + the automatic adding of the DefaultComponent. I might want to handle this some other way.
		Entity AddEntity(const std::string& name);

		void RemoveEntity(EntityID id);
		Entity DuplicateEntity(EntityID id);

		Entity GetEntity(EntityID id);

		template<typename Comp, typename ...Args>
		Comp* AddComponent(const EntityID id, Args&&... compArgs);

		template<typename Comp>
		void RemoveComponent(const EntityID id);

		template<typename Comp>
		bool HasComponent(const EntityID& id);

		template<typename Comp>
		Comp* GetComponent(const EntityID& id);

		/// <summary>
		/// Checks whether an entity with a specific ID exists.
		/// </summary>
		/// <returns>Returns true if an entity with EntityID exists.</returns>
		bool IsValid(EntityID id);

		/// <summary>
		/// Return the number of active entities
		/// </summary>
		TypeID Count() const;

		void RemoveAll();

		template<typename ...Comps>
		typename std::enable_if<(sizeof...(Comps) > 0), void>::type
		ForEach(std::function<void(EntityID, Comps*...)> func);

		template<typename ...Comps>
		typename std::enable_if<(sizeof...(Comps) == 0), void>::type
		ForEach(std::function<void(EntityID)> func);

		template<typename ...Comps>
		std::vector<EntityID> QueryEntities();


		class ArchetypeRange;
		/// <summary>
		/// Returns a range of archetypes
		/// </summary>
		ArchetypeRange Range();

	private:
		class ArchetypeRange 
		{
			using ArchContainer = std::vector<Archetype*>;
			class Iterator 
			{
			public:
				using difference_type = std::ptrdiff_t;
				using iterator_category = std::bidirectional_iterator_tag;
				using InnerIterator = std::vector<Archetype*>::iterator;

				explicit Iterator(InnerIterator it);

				Archetype &operator*() const;
				Archetype *operator->() const;

				Iterator& operator++();
				Iterator& operator--();
				Iterator operator++(int);
				Iterator operator--(int);

				friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_Ptr == b.m_Ptr; }
				friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_Ptr != b.m_Ptr; }

			private:
				InnerIterator m_Ptr;
			};

		public:
			explicit ArchetypeRange(ArchContainer& container);

			Iterator begin() const;
			Iterator end() const;

		private:
			ArchContainer& m_Container;
		};

	private:
		// Forward declaring some stuff that we'll need later...
		struct EntityRecord;
		struct ComponentData;

		Archetype* CreateArchetype(const ArchetypeID& archetypeID);
		Archetype* GetArchetype(const ArchetypeID& archetypeID);

		template<typename Comp>
		static const ComponentID GetComponentID();

		template<typename Comp>
		ComponentData* RegisterComponent();

		void RemoveEntityFromArchetype(EntityID id, Archetype& archetype);
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