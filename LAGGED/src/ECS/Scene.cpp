#include "Precomp.h"
#include "Scene.h"

namespace LAG
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::AddEntity()
	{
		entt::entity newEntity = m_Registry.create();
		return Entity(newEntity, m_Registry);
	}

	void Scene::RemoveEntity(uint32_t entityID)
	{
		entt::entity entity = static_cast<entt::entity>(entityID);
		if (m_Registry.valid(entity))
			m_Registry.destroy(entity);
		else Utility::Logger::Warning("Tried to remove an entity that is invalid.");
	}

	bool Scene::DoesEntityExist(uint32_t entityID)
	{
		return m_Registry.valid(static_cast<entt::entity>(entityID));
	}

	Entity Scene::GetEntity(uint32_t entityID)
	{
		return Entity(static_cast<entt::entity>(entityID), m_Registry);
	}

	void Scene::RemoveAll()
	{
		if (m_Registry.storage<entt::entity>().size() > 0)
		{
			for (const auto entity : m_Registry.storage<entt::entity>())
				RemoveEntity(static_cast<uint32_t>(entity));
		}
	}
}