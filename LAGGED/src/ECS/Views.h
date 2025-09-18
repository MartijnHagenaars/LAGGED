#pragma once
#include <vector>
#include <string>

#include "Containers.h"
#include "TypeDefines.h"

namespace LAG
{
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

	class MemberView;
	class ComponentView
	{
		class MemberRange;
	public:
		ComponentView() = delete;
		// TODO: Consider removing Scene reference
		ComponentView(Scene& scene, ComponentID id, ComponentData& compData);

		MemberRange Members();

		void* GetVoid(EntityID id);
		std::any ToAny(void* data);

		size_t Size() const { return m_ComponentData.size; }
		const std::string_view Name() const { return m_ComponentData.debugName; }

	private:
		class MemberRange
		{
			using DataContainer = std::vector<ReflectionData::MemberData>;
			class Iterator
			{
			public:
				using difference_type = std::ptrdiff_t;
				using iterator_category = std::bidirectional_iterator_tag;
				using InnerIterator = DataContainer::iterator;

				explicit Iterator(InnerIterator it);

				MemberView operator*() const;
				MemberView operator->() const;
				 
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
			MemberRange() = delete;
			MemberRange(DataContainer& dataContainer);

			Iterator begin() const { return Iterator(m_Container.begin()); }
			Iterator end() const { return Iterator(m_Container.end()); }

		private:
			DataContainer& m_Container;
		};

	private:
		Scene& m_Scene;
		ComponentID m_ID;
		ComponentData& m_ComponentData;
		ReflectionData& m_ReflectionData;

		//Something where variables are stored
	};

	class MemberView
	{
	public:
		MemberView() = delete;
		MemberView(ReflectionData::MemberData& memberData);

		ReflectionData::MemberData::Properties& Properties() const { return m_MemberData.props; }

	private:
		ReflectionData::MemberData& m_MemberData;
	};
}