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
			using CompIdContainer = std::vector<TypeID>;
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
		ComponentView(Scene& scene, TypeID id, TypeInfo& compData);

		ReflectedCompInfo::Properties& Properties() const { return m_ReflectionData.props; }

		MemberRange Members();

		void* GetVoid(EntityID id);
		std::any ToAny(void* data);

		size_t Size() const { return m_ComponentData.size; }

#ifdef DEBUG
		const std::string_view Name() const { return m_ComponentData.debugName; }
#else
		const std::string_view Name() const { return std::string(); }
#endif	

	private:
		class MemberRange
		{
			using DataContainer = std::vector<ReflectedCompInfo::MemberInfo>;
			class Iterator
			{
			public:
				using difference_type = std::ptrdiff_t;
				using iterator_category = std::bidirectional_iterator_tag;
				using InnerIterator = DataContainer::iterator;

				explicit Iterator(InnerIterator it, ComponentView& parentCompView);

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
				ComponentView& m_ParentCompView;
			};

		public:
			MemberRange() = delete;
			MemberRange(DataContainer& dataContainer, ComponentView& parentCompView);

			Iterator begin() const { return Iterator(m_Container.begin(), m_ParentCompView); }
			Iterator end() const { return Iterator(m_Container.end(), m_ParentCompView); }

		private:
			DataContainer& m_Container;
			ComponentView& m_ParentCompView;
		};

	private:
		Scene& m_Scene;
		TypeID m_ID;
		TypeInfo& m_ComponentData;
		ReflectedCompInfo& m_ReflectionData;

		//Something where variables are stored
	};

	class MemberView
	{
	public:
		MemberView() = delete;
		MemberView(ReflectedCompInfo::MemberInfo& memberData, TypeInfo& typeInfo, ComponentView& parentCompView);

		ReflectedCompInfo::MemberInfo::Properties& Properties() const { return m_MemberData.props; }

		void* GetVoid(EntityID id) const;
		std::any ToAny(void* data) const;

		class ReflectionFuncs
		{
		public:
			ReflectionFuncs() = delete;
			ReflectionFuncs(std::function<void(const std::vector<std::any>&)> func) :
				m_Func(func)
			{}

			explicit operator bool() const { return (m_Func != nullptr); }

			template<typename... Args>
			void Invoke(Args&&... args)
			{
				m_Func({ args... });
			}

		private:
			std::function<void(const std::vector<std::any>&)> m_Func;
		};
		ReflectionFuncs Func(Hash64 funcNameHash) const;


	private:
		ReflectedCompInfo::MemberInfo& m_MemberData;
		TypeInfo& m_TypeInfo;
		ComponentView& m_ParentCompView;
	};
}