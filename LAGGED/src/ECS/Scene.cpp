#include "Scene.h"

#include "Components/BasicComponents.h"

#include "ImGui/imgui.h"
#include "Meta/ReflectionComponentSetup.h"

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
		//TODO: Implement function...
		return Entity();
	}

	Entity Scene::AddEntity(const std::string& entityName)
	{
		//TODO: Implement function...
		return Entity();
	}

	Entity Scene::DuplicateEntity(uint32_t entityID)
	{
		//TODO: Implement function...
		return Entity();
	}

	void Scene::RemoveEntity(uint32_t entityID)
	{
		//TODO: Implement function...
	}

	bool Scene::DoesEntityExist(uint32_t entityID)
	{
		//TODO: Implement function...
		return false;
	}

	Entity Scene::GetEntity(uint32_t entityID)
	{
		//TODO: Implement function...
		return Entity();
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


	///////////////////////////////////////////////////////////////
	// Functionality for Entt Meta widgets, used in Editor Tools //
	///////////////////////////////////////////////////////////////

	void Scene::HandleComponentWidgets(Entity* entity, Reflection::WidgetModes mode)
	{

	}

	void Scene::ComponentLoop(std::function<void(Component& comp)> func)
	{
		//TODO: Implement function...
	}
}