#include "Scene.h"

#include "Components/BasicComponents.h"

#include "ImGui/imgui.h"
#include "Meta/ReflectionDefines.h"

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
		m_Registry.emplace<DefaultComponent>(newEntity, "New entity");
		return Entity(newEntity, m_Registry);
	}

	Entity Scene::AddEntity(const std::string& entityName)
	{
		entt::entity newEntity = m_Registry.create();
		m_Registry.emplace<DefaultComponent>(newEntity, entityName);
		return Entity(newEntity, m_Registry);
	}

	void Scene::RemoveEntity(uint32_t entityID)
	{
		entt::entity entity = static_cast<entt::entity>(entityID);
		if (m_Registry.valid(entity))
			m_Registry.destroy(entity);
		else Logger::Warning("Tried to remove an entity that is invalid.");
	}

	bool Scene::DoesEntityExist(uint32_t entityID)
	{
		return m_Registry.valid(static_cast<entt::entity>(entityID));
	}

	Entity Scene::GetEntity(uint32_t entityID)
	{
		return Entity(static_cast<entt::entity>(entityID), m_Registry);
	}

	size_t Scene::Count() const
	{
		return m_Registry.storage<entt::entity>()->size();
	}

	void Scene::RemoveAll()
	{
		if (m_Registry.storage<entt::entity>().size() > 0)
		{
			for (const auto entity : m_Registry.storage<entt::entity>())
				RemoveEntity(static_cast<uint32_t>(entity));
		}
	}


	///////////////////////////////////////////////////////////////
	// Functionality for Entt Meta widgets, used in Editor Tools //
	///////////////////////////////////////////////////////////////

	void Scene::HandleComponentWidgets(Entity* entity, Reflection::WidgetModes mode)
	{
		for (auto&& [idType, storageSet] : m_Registry.storage())
		{
			if (storageSet.contains(entt::entity(entity->GetEntityID())))
			{
				auto componentMeta = entt::resolve(storageSet.type());
				if (!componentMeta)
					continue;

				std::string compName = std::string(componentMeta.info().name());
				ImGui::Text(compName.c_str());

				if (!ReflectComponent(componentMeta, storageSet, entity->m_EntityID, mode))
					ImGui::Text("Failed to load reflection data for component.");
				else ImGui::Separator();
			}
		}
	}

	bool Scene::ReflectComponent(entt::meta_type& compMeta, entt::sparse_set& storageSet, entt::entity entity, Reflection::WidgetModes mode)
	{
		entt::meta_any compElement = compMeta.from_void(storageSet.value(entity));
		if (compElement == nullptr)
			return false;

		for (auto&& [idType, propMetaData] : compMeta.data())
		{
			entt::meta_any propInstance, propInstanceCompare;
			propInstanceCompare = propInstance = propMetaData.get(compElement);
			ReflectProperty(propMetaData, propInstance, mode);

			//Check if the component has been modified. If so, (re)set the values
			if (propInstance != propInstanceCompare)
				propMetaData.set(compElement, propInstance);
		}

		return true;
	}

	void Scene::ReflectProperty(entt::meta_data& propData, entt::meta_any& propValues, Reflection::WidgetModes mode)
	{
		std::string propDisplayName;
		entt::meta_prop displayNameProp = propData.prop(Reflection::DISPLAY_NAME);
		if (displayNameProp.value() != nullptr)
		{
			propDisplayName = displayNameProp.value().cast<std::string>();
			ImGui::PushID(std::string(std::to_string(propValues.type().id()) + propDisplayName).c_str());
			ReflectType(propData, propValues, propDisplayName, mode);
			ImGui::PopID();
		}
		else
			propDisplayName = "Undefined property display name";
	}

	void Scene::ReflectType(entt::meta_data& typeData, entt::meta_any& typeValues, const std::string& propName, Reflection::WidgetModes mode)
	{
		if (typeValues.type().func(Reflection::EDITOR_WIDGET_DRAW))
		{
			typeValues.type().func(Reflection::EDITOR_WIDGET_DRAW).invoke<const char*, entt::meta_any, entt::meta_data&>({ 0 },
				propName.c_str(), entt::forward_as_meta(typeValues), typeData, mode
			);
		}
		else
			ImGui::Text(std::string("No meta inspect function detected for " + propName).c_str());
	}
}