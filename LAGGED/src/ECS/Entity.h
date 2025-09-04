#pragma once
#include "ECS/Scene.h"
#include <memory>

#include "ECS/Reflection/SceneReflect.h"

// TODO: CONSIDER REMOVING THE ENTITY CLASS. IT'S MOSTLY UNUSED.

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

	private:
		friend class Scene;
		Entity(Scene& sceneRef, EntityID entityID);

		EntityID m_ID = ENTITY_NULL;
		Scene* m_SceneRef = nullptr;
	};
}

// Implement the Entity template functions
#include "Entity.inl"