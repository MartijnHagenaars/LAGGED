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

	bool Scene::DoesEntityExist(uint32_t entityID)
	{
		return m_Registry.valid(static_cast<entt::entity>(entityID));
	}
}