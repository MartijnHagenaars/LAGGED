#pragma once
#include <vector>
#include <string>

#include "Containers.h"
#include "TypeDefines.h"

namespace LAG
{
	class Scene;
	class ComponentView;
	class FunctionView;
	class ArchetypeView
	{
		class ComponentRange;
	public:
		ArchetypeView() = delete;
		explicit ArchetypeView(Archetype& archetype);

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

				explicit Iterator(InnerIterator it);

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
			};

		public:
			ComponentRange() = delete;
			explicit ComponentRange(CompIdContainer& idContainer);

			Iterator begin() const { return Iterator(m_IdContainer.begin()); }
			Iterator end() const { return Iterator(m_IdContainer.end()); }

		private:
			CompIdContainer& m_IdContainer;
		};

		Archetype& m_Archetype;
	};

	class MemberView;
	class ComponentView
	{
		class MemberRange;
	public:
		ComponentView() = delete;
		ComponentView(TypeID id, TypeInfo& compData);

		ReflectionCompInfo::Properties* Props() const;

		MemberRange Members();

		TypeID ID() const { return m_ID; }
		void* GetVoid(EntityID id);
		std::any ToAny(void* data);

		FunctionView Func(Hash64 funcNameHash) const;

		size_t Size() const { return m_ComponentData.size; }

#ifdef DEBUG
		const std::string_view Name() const { return m_ComponentData.debugName; }
#else
		const std::string_view Name() const { return std::string_view("UNDEF"); }
#endif	

	private:
		class MemberRange
		{
			using DataContainer = std::vector<ReflectionCompInfo::MemberInfo>;
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
		TypeID m_ID;
		TypeInfo& m_ComponentData;

		ReflectionCompInfo* m_ReflectionData;
	};

	class MemberView
	{
	public:
		MemberView() = delete;
		MemberView(ReflectionCompInfo::MemberInfo& memberData, TypeInfo& typeInfo, ComponentView& parentCompView);

		ReflectionCompInfo::MemberInfo::Properties& Props() const { return m_MemberData.props; }

		void* GetVoid(EntityID id) const;
		std::any ToAny(void* data) const;

		FunctionView Func(Hash64 funcNameHash) const;

	private:
		ReflectionCompInfo::MemberInfo& m_MemberData;
		TypeInfo& m_TypeInfo;
		ComponentView& m_ParentCompView;
	};

	class FunctionView
	{
		using FuncHashMap = std::unordered_map<Hash64, std::function<void(const std::vector<std::any>&)>>;
	public:
		FunctionView() = delete;
		FunctionView(FuncHashMap& funcMap, Hash64 funcNameHash)
		{
			auto funcIt = funcMap.find(funcNameHash);
			if (funcIt == funcMap.end())
			{
				INFO("No reflection function found for function with hash {}", funcNameHash);
				m_Func = nullptr;
			}
			else 
				m_Func = funcIt->second;
		}

		template<typename... Args>
		void Invoke(Args&&... args)
		{
			if (!m_Func)
				CRITICAL("Tried to invoke invalid reflection function.");
			m_Func({ args... });
		}

		bool Valid() { return m_Func != nullptr; }

	private:
		std::function<void(const std::vector<std::any>&)> m_Func;
	};
}