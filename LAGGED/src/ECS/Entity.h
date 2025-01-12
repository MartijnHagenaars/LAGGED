#pragma once
#include "Utility/Logger.h"
#include "ECS/TypeDefines.h"

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

		//Adds the specified component to the entity. 
		template<typename T, typename ... Args>
		T* AddComponent(Args&&... arguments)
		{
			//TODO: Implement function...
			return nullptr;
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
		Entity(EntityID entityID, Scene& sceneRef);

		EntityID m_ID;
		Scene* m_SceneRef;
	};
}
