#pragma once
#include <algorithm>
#include <functional>

#include "Containers.h"
#include "TypeDefines.h"
#include "Views.h"

#include "Utility/Hash.h"
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
		class ArchetypeRange;
		friend class SceneReflect;
		friend class ArchetypeView;
		friend class ComponentView;
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

		// Helper function for getting the hash64 of a type
		template<typename T>
		static constexpr TypeID GetTypeID();

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
				using InnerIterator = ArchContainer::iterator;

				explicit Iterator(Scene& scene, InnerIterator it);

				ArchetypeView operator*() const;
				ArchetypeView operator->() const;

				Iterator& operator++();
				Iterator& operator--();
				Iterator operator++(int);
				Iterator operator--(int);

				friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_Ptr == b.m_Ptr; }
				friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_Ptr != b.m_Ptr; }

			private:
				Scene& m_Scene;
				InnerIterator m_Ptr;
			};

		public:
			explicit ArchetypeRange(Scene& scene, ArchContainer& container);

			Iterator begin() const;
			Iterator end() const;

		private:
			Scene& m_Scene;
			ArchContainer& m_Container;
		};

	private:
		// Forward declaring some stuff that we'll need later...
		// TODO: Maybe move this to the Containers header?
		struct EntityRecord;

		Archetype* CreateArchetype(const ArchetypeID& archetypeID);
		Archetype* GetArchetype(const ArchetypeID& archetypeID);

		template<typename Type>
		static TypeInfo& RegisterType();

		void RemoveEntityFromArchetype(EntityID id, Archetype& archetype);
		void ShrinkComponentBuffer(Archetype& archetype, const EntityRecord& entityRecord);
		void ResizeAndReallocateComponentBuffer(Archetype& archetype, const TypeInfo& componentData, int componentIndex, size_t targetSize);

	private:
		inline static EntityID s_EntityCounter = 0;

		// This vector stores all possible archetypes
		std::vector<Archetype*> m_Archetypes;

		struct EntityRecord
		{
			size_t index;
			Archetype* archetype;
		};
		// This map stores all entities and the archetypes that they use.
		std::unordered_map<EntityID, EntityRecord> m_EntityArchetypes;

		// TODO: Good description.
		inline static std::unordered_map<Hash64, TypeInfo> s_TypeInfo;

		// This map links a (Component-class) TypeID to its Component-type reflection info
		inline static std::unordered_map<TypeID, ReflectedCompInfo> s_ReflectedCompInfo;

	};
}

// Implement the Scene template functions
#include "Scene.inl"