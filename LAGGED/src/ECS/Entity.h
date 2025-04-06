#pragma once
#include "ECS/Scene.h"

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
		bool HasComponent();

		template<typename Comp>
		Comp* GetComponent();

		bool IsValid();

		EntityID GetEntityID() const;

	private:
		friend class Scene;
		Entity(Scene& sceneRef, EntityID entityID);

		EntityID m_ID = ENTITY_NULL;
		Scene* m_SceneRef = nullptr;
	};
}

// Implement the Entity template functions
#include "Entity.inl"