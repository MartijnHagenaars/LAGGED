#pragma once
#include "ECS/Scene.h"
#include <memory>

#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
	class Scene;
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& entity) = default;

		template<typename Comp, typename ...Args>
		Comp* AddComponent(Args && ...args);

		template<typename Comp>
		void RemoveComponent();

		template<typename Comp>
		bool HasComponent() const;

		template<typename Comp>
		Comp* GetComponent() const;

		bool Valid() const;

		EntityID ID() const;

		struct Iterator
		{
			Iterator(ComponentID* ptr) :
				m_IdPtr(ptr)
			{}

			SceneReflect::ComponentData* operator*() const { return &SceneReflect::GetComponentProperties().at(*m_IdPtr); }
			SceneReflect::ComponentData* operator->() { return &SceneReflect::GetComponentProperties().at(*m_IdPtr); }

			Iterator& operator++()
			{
				m_IdPtr++;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator tempIt = *this;
				++(*this);
				return tempIt;
			}

			Iterator& operator--()
			{
				m_IdPtr--;
				return *this;
			}

			Iterator operator--(int)
			{
				Iterator tempIt = *this;
				--(*this);
				return tempIt;
			}

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_IdPtr == b.m_IdPtr; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_IdPtr != b.m_IdPtr; }

		private:
			ComponentID* m_IdPtr;
		};

		Iterator begin() 
		{ 
			ArchetypeID& archetype = m_SceneRef->m_EntityArchetypes.at(m_ID).archetype->typeID;
			return archetype.empty() ? end() : Iterator(&archetype[0]);
		}
		Iterator end() 
		{ 
			ArchetypeID& archetype = m_SceneRef->m_EntityArchetypes.at(m_ID).archetype->typeID;
			return archetype.empty() ? end() : Iterator(&archetype[archetype.size() - 1]);
		}

	private:
		friend class Scene;
		Entity(Scene& sceneRef, EntityID entityID);

		EntityID m_ID = ENTITY_NULL;
		Scene* m_SceneRef = nullptr;
	};
}

// Implement the Entity template functions
#include "Entity.inl"