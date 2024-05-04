#pragma once
#include "entt/entt.hpp"
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
			if (m_RegistryPtr->valid(m_EntityID))
				return m_RegistryPtr->try_get<T>(m_EntityID);
			else return false;
		}

		//Adds the specified component to the entity. 
		template<typename T, typename ... Args>
		T* AddComponent(Args&&... arguments)
		{
			if (!HasComponent<T>())
			{
				m_RegistryPtr->emplace<T>(m_EntityID, std::forward<Args>(arguments)...);
				return &m_RegistryPtr->get<T>(m_EntityID);
			}
			
			Logger::Warning("Tried to add a component to an entity that already has the same component.");
			return &m_RegistryPtr->get<T>(m_EntityID);
		}

		void AddComponent(uint32_t componentID);

		template<typename T>
		void RemoveComponent()
		{
			if (HasComponent<T>())
				m_RegistryPtr->remove<T>(m_EntityID);
			else 
				Logger::Warning("Failed to remove component: entity doesn't have component.");
		}

		template<typename T>
		T* GetComponent()
		{
			if (m_RegistryPtr->valid(m_EntityID))
			{
				if (HasComponent<T>())
					return &m_RegistryPtr->get<T>(m_EntityID);
			}
			else Logger::Error("Tried to get an invalid entity.");

			return nullptr;
		}

		uint32_t GetEntityID() const;

	private:
		friend class Scene;
		Entity(entt::entity entityID, entt::registry& registryPtr);

		entt::entity m_EntityID = entt::tombstone;
		entt::registry* m_RegistryPtr = nullptr;

	};
}
