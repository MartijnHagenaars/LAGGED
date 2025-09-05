#pragma once

#include <functional>
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

		std::function<void(unsigned char* dest)> CreateObjectInMemory;
		std::function<void(unsigned char* src, unsigned char* dest)> MoveData;
		std::function<void(unsigned char* data)> DestructData;
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
			using CompDataContainer = std::unordered_map<ComponentID, ComponentData*>;
			class Iterator
			{
			public:
				using difference_type = std::ptrdiff_t;
				using iterator_category = std::bidirectional_iterator_tag;
				using InnerIterator = CompIdContainer::iterator;

				explicit Iterator(InnerIterator it, CompDataContainer& compData);

				ComponentView operator*() const;
				ComponentView operator->() const;

				Iterator& operator++();
				Iterator& operator--();
				Iterator operator++(int);
				Iterator operator--(int);

				friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_Ptr == b.m_Ptr; }
				friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_Ptr != b.m_Ptr; }

			private:
				InnerIterator m_Ptr;
				CompDataContainer& m_ComponentData;
			};

		public:
			ComponentRange() = delete;
			ComponentRange(CompIdContainer& idContainer, CompDataContainer& dataContainer);

			Iterator begin() const { return Iterator(m_IdContainer.begin(), m_DataContainer); }
			Iterator end() const { return Iterator(m_IdContainer.end(), m_DataContainer); }

		private:
			CompIdContainer& m_IdContainer;
			CompDataContainer& m_DataContainer;
		};

		Scene& m_Scene;
		Archetype& m_Archetype;
	};

	class ComponentView
	{
	public: 
		ComponentView() = delete;
		ComponentView(ComponentData& compData);

		size_t Size() const { return m_ComponentData.size; }
		const std::string_view Name() const { return m_ComponentData.debugName; }

	private:
		ComponentData& m_ComponentData;
	};
}