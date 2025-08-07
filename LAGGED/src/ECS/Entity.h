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

			SceneReflect::ComponentData* operator*() const;
			SceneReflect::ComponentData* operator->();

			Iterator& operator++();
			Iterator operator++(int);
			Iterator& operator--();
			Iterator operator--(int);

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_IdPtr == b.m_IdPtr; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_IdPtr != b.m_IdPtr; }

		private:
			ComponentID* m_IdPtr;
		};

		Iterator begin();
		Iterator end();

	private:
		friend class Scene;
		Entity(Scene& sceneRef, EntityID entityID);

		EntityID m_ID = ENTITY_NULL;
		Scene* m_SceneRef = nullptr;
	};
}

// Implement the Entity template functions
#include "Entity.inl"