#pragma once
#include "Utility/Logger.h"

namespace LAG
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& entity);

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

		uint32_t GetEntityID() const;

	private:
		friend class Scene;
		Entity(entt::entity entityID, entt::registry& registryPtr);
	};
}
