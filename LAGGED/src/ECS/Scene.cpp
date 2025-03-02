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

		ArchetypeRecord rec = {};
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
		return (m_EntityArchetypes.find(entityID) != m_EntityArchetypes.end());
	}

	size_t Scene::Count() const
	{
		return m_EntityArchetypes.size();
	}

	void Scene::RemoveAll()
	{
		//TODO: Implement function...
	}


	// ======== ARCHETYPES ========

	Archetype* Scene::CreateArchetype(const ArchetypeID& archetypeID)
	{
		Archetype* archetype = new Archetype();
		archetype->typeID = archetypeID;

		m_Archetypes.emplace_back(archetype);

		for (int i = 0; i < archetypeID.size(); i++)
		{
			archetype->compData.push_back(nullptr);
			archetype->compDataSize.push_back(0);
		}

		return archetype;
	}

	Archetype* Scene::GetArchetype(const ArchetypeID& archetypeID)
	{
		for (auto* archetype : m_Archetypes)
		{
			if (archetype->typeID == archetypeID)
				return archetype;
		}

		return nullptr;
	}
}