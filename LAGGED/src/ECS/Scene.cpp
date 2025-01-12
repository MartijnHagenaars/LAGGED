#include "Scene.h"

#include "Components/BasicComponents.h"

#include "ImGui/imgui.h"

namespace LAG
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity* Scene::AddEntity()
	{
		//TODO: Implement function...
		return nullptr;
	}

	Entity* Scene::AddEntity(const std::string& entityName)
	{
		//TODO: Implement function...
		return nullptr;
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

	Entity* Scene::GetEntity(EntityID entityID)
	{
		//TODO: Implement function...
		return nullptr;
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