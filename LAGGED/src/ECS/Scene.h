#pragma once
#include <algorithm>
#include <functional>

#include "Containers.h"
#include "TypeDefines.h"
#include "Views.h"

namespace LAG
{
	namespace Reflection
	{
		enum class WidgetModes;
	}

	class Scene
	{
		class ArchetypeRange;
		friend class SceneReflect;
		friend class ArchetypeView;
		friend class ComponentView;
	public:
		Scene();
		~Scene();

		EntityID AddEntity();

		// TODO: Consider removing this + the automatic adding of the EditorComponent in specific Game/Shipping configurations
		EntityID AddEntity(const std::string& name);

		void RemoveEntity(EntityID id);
		EntityID DuplicateEntity(EntityID id);

		// Add a component to an entity using a component template type.
		template<typename Comp, typename ...Args>
		Comp* AddComponent(EntityID id, Args&&... compArgs);
		// Add a component to an entity using a component type ID. 
		void* AddComponent(EntityID id, TypeID compID);

		// Remove a component from an entity using a component template type. 
		template<typename Comp>
		void RemoveComponent(EntityID id);
		// Remove a component from an entity using a component type ID. 
		void RemoveComponent(EntityID id, TypeID compID);

		// Check if an entity contains a specific component using a component template type.
		template<typename Comp>
		bool HasComponent(EntityID id);
		// Check if an entity contains a specific component using a component type ID. 
		bool HasComponent(EntityID id, TypeID compID);

		template<typename Comp>
		Comp* GetComponent(EntityID id);

		/// <summary>
		/// Checks whether an entity with a specific ID exists.
		/// </summary>
		/// <returns>Returns true if an entity with EntityID exists.</returns>
		bool IsValid(EntityID id);

		/// <summary>
		/// Return the number of active entities
		/// </summary>
		size_t Count() const;

		void RemoveAll();

		template<typename ...Comps>
		typename std::enable_if<(sizeof...(Comps) > 0), void>::type
		ForEach(std::function<void(EntityID, Comps*...)> func);

		template<typename ...Comps>
		typename std::enable_if<(sizeof...(Comps) == 0), void>::type
		ForEach(std::function<void(EntityID)> func);

		template<typename ...Comps>
		std::vector<EntityID> QueryEntities();

		/// <summary>
		/// Returns a range of archetypes
		/// </summary>
		ArchetypeRange Range();

		/// <summary>
		/// Query typeIDs of all reflected components
		/// </summary>
		/// <returns>A vector containing all component IDs</returns>
		static std::vector<TypeID> QueryCompIDs();

		// Helper function for getting the hash64 of a type
		template<typename T>
		static constexpr TypeID GetTypeID();

	private:
		class ArchetypeRange 
		{
			using ArchContainer = std::vector<Archetype*>;
			class Iterator 
			{
			public:
				using difference_type = std::ptrdiff_t;
				using iterator_category = std::bidirectional_iterator_tag;
				using InnerIterator = ArchContainer::iterator;

				explicit Iterator(InnerIterator it);

				ArchetypeView operator*() const;
				ArchetypeView operator->() const;

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
		Archetype* CreateArchetype(const ArchetypeID& archetypeID);
		Archetype* GetArchetype(const ArchetypeID& archetypeID);

		template<typename Type>
		static TypeInfo& RegisterType();

		void RemoveEntityFromArchetype(EntityID id, Archetype& archetype);
		void ShrinkComponentBuffer(Archetype& archetype, const EntityRecord& entityRecord);
		void ResizeAndReallocateComponentBuffer(Archetype& archetype, const TypeInfo& typeInfo, int componentIndex, size_t targetSize);

	private:
		inline static EntityID s_EntityCounter = 0;

		// This vector stores all possible archetypes
		std::vector<Archetype*> m_Archetypes;

		// This map stores all entities and the archetypes that they use.
		std::unordered_map<EntityID, EntityRecord> m_EntityArchetypes;

		/// <summary>
		/// Data structure that contains useful information and functions for data types.
		/// Implemented as a getter-function to work around SIOF issue
		/// </summary>
		/// <returns>Unordered_map, where the key is the TypeID / TypeHash64 of the type</returns>
		static std::unordered_map<Hash64, TypeInfo>& GetTypeInfo()
		{
			static std::unordered_map<Hash64, TypeInfo> map;
			return map;
		}

		// This map links a (Component-class) TypeID to its Component-type reflection info
		inline static std::unordered_map<TypeID, ReflectionCompInfo> s_ReflectedCompInfo;

	};
}

// Implement the Scene template functions
#include "Scene.inl"