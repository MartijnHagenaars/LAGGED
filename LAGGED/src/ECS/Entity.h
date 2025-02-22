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

		bool IsValid();

		template<typename T>
		bool HasComponent()
		{
			//TODO: Implement function...
			return false;
		}

		template<typename Comp, typename ...Args>
		Comp* AddComponent(Args && ...args)
		{
			return m_SceneRef->AddComponent<Comp>(m_ID, args...);
		}

		template<typename T>
		void RemoveComponent()
		{
			//TODO: Implement function...
		}

		template<typename T>
		T* GetComponent()
		{
			//TODO: Implement function...
			return nullptr;
		}

		EntityID GetEntityID() const;

	private:
		friend class Scene;
		Entity(Scene& sceneRef, EntityID entityID);

		EntityID m_ID = ENTITY_NULL;
		Scene* m_SceneRef = nullptr;
	};
}
