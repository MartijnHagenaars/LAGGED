#pragma once

#include <any>
#include <string>
#include <unordered_map>
#include <vector>

#include "ECS/TypeDefines.h"

namespace LAG
{
	struct Archetype
	{
#ifdef DEBUG
		std::string debugName;
#endif

		ArchetypeID typeID; //ID of the archetype
		std::vector<EntityID> entityIDs; //EntityIDs that belong to this archetype

		std::vector<unsigned char*> compData; //The data of all the components
		std::vector<size_t> compDataSize; //The size of all components

		std::unordered_map<ComponentID, size_t> systemCompIndices; //Determines the order of components when running a system
	};

	struct ComponentData
	{
		size_t size = -1;

		void(*CreateObjectInMemory)(unsigned char* dest);
		void(*MoveData)(unsigned char* src, unsigned char* dest);
		void(*DestructData)(unsigned char* data);

		std::any(*VoidToAny)(void*) = nullptr;
#ifdef DEBUG
		std::string debugName;
#endif
	};

	// Proxy class for Archetype
	class Scene;
	class ComponentView;
	class ArchetypeView
	{
		class ComponentRange;
	public:
		ArchetypeView() = delete;
		ArchetypeView(Scene& scene, Archetype& archetype);

	public:
		ComponentRange Types();

		bool Contains(EntityID id) const;

	private:
		class ComponentRange
		{
			using CompIdContainer = std::vector<ComponentID>;
			class Iterator
			{
			public:
				using difference_type = std::ptrdiff_t;
				using iterator_category = std::bidirectional_iterator_tag;
				using InnerIterator = CompIdContainer::iterator;

				explicit Iterator(Scene& scene, InnerIterator it);

				ComponentView operator*() const;
				ComponentView operator->() const;

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
			ComponentRange() = delete;
			ComponentRange(Scene& scene, CompIdContainer& idContainer);

			Iterator begin() const { return Iterator(m_Scene, m_IdContainer.begin()); }
			Iterator end() const { return Iterator(m_Scene, m_IdContainer.end()); }

		private:
			Scene& m_Scene;
			CompIdContainer& m_IdContainer;
		};

		Scene& m_Scene;
		Archetype& m_Archetype;
	};

	class ComponentView
	{
	public: 
		ComponentView() = delete;
		ComponentView(Scene& scene, ComponentID id, ComponentData& compData);

		void* GetVoid(EntityID id);
		std::any ToAny(void* data);

		size_t Size() const { return m_ComponentData.size; }
		const std::string_view Name() const { return m_ComponentData.debugName; }

	private:
		Scene& m_Scene;
		ComponentID m_ID;
		ComponentData& m_ComponentData;
	};
}