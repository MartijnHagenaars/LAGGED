#include "Scene.h"
#include "ECS/Entity.h"

#include "Components/BasicComponents.h"

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
		EntityID newEntityID = ++s_EntityCounter;

		auto entArchetype = m_EntityArchetypes.find(newEntityID);
		if (entArchetype != m_EntityArchetypes.end())
			ERROR("Entity with ID {} already exists in EnityArchetypes map.", newEntityID);

		Record rec = {};
		rec.index = 0;
		rec.archetype = nullptr;
		m_EntityArchetypes.insert({ newEntityID, rec });

		return Entity(*this, newEntityID);
	}

	Entity Scene::AddEntity(const std::string& entityName)
	{
		Entity newEntity = AddEntity();
		newEntity.AddComponent<DefaultComponent>(entityName);
		
		return newEntity;
	}

	Entity* Scene::DuplicateEntity(EntityID entityID)
	{
		//TODO: Implement function...
		return nullptr;
	}

	void Scene::RemoveEntity(EntityID entityID)
	{
		//TODO: Implement function...
	}

	bool Scene::DoesEntityExist(EntityID entityID)
	{
		//TODO: Implement function...
		return false;
	}

	size_t Scene::Count() const
	{
		//TODO: Implement function...
		return 0;
	}

	void Scene::RemoveAll()
	{
		//TODO: Implement function...
	}
}