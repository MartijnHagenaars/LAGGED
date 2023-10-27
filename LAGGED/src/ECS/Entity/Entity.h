#pragma once
#include "entt/entt.hpp"

namespace LAG
{
	class Entity
	{
	public:
		Entity() = delete;
		Entity(const Entity&) = delete;
		~Entity();

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
				//std::forward stuff: https://stackoverflow.com/questions/65144882/how-to-pass-any-number-of-arguments-to-c-template-function
				m_RegistryPtr->emplace<T>(m_EntityID, std::forward<Args>(arguments)...);
				return m_RegistryPtr->get<T>(m_EntityID);

			}
			
			Utility::Logger::Warning("Tried to add a component to an entity that already has the same component.");
			return m_RegistryPtr->get<T>(m_EntityID);
		}

	private:
		friend class Scene;
		Entity(entt::entity entityID, entt::registry& registryPtr);

		entt::entity m_EntityID;
		entt::registry* m_RegistryPtr = nullptr;

	};
}