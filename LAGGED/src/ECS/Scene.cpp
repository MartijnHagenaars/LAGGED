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
		for (const auto& it : m_Registry.storage())
		{
			entt::sparse_set& storageSet = it.second;
			if (storageSet.contains(entt::entity(entity->GetEntityID())))
			{
				//Get the component meta for this specific type and check if it's valid. 
				entt::meta_type compMeta = entt::resolve(storageSet.type());
				if (compMeta)
				{
					auto compElement = storageSet.value(entt::entity(entity->GetEntityID()));
					entt::meta_any compInstance = compMeta.from_void(compElement);
					if (compInstance != nullptr)
					{
						//Get the display name of the component
						std::string compName;
						entt::meta_prop compDisplayNameProp = compMeta.prop(Reflection::ComponentProperties::DISPLAY_NAME);
						if (compDisplayNameProp)
							compName = compDisplayNameProp.value() != nullptr ? compDisplayNameProp.value().cast<std::string>() : std::string(compMeta.info().name());
						else compName = std::string(compMeta.info().name());

						ImGui::SeparatorText(compName.c_str());
						ReflectComponent(compMeta, compInstance, entity, mode);

					}
					else
					{
						ImGui::SeparatorText(std::string(compMeta.info().name()).c_str());
						ImGui::Text("Failed to load reflection data for component. Has the component reflection been set up?");
					}
				}
				else printf("???\n");
			}
		}
	}

	bool Scene::ReflectComponent(entt::meta_type& compMeta, entt::meta_any& compInstance, Entity* entity, Reflection::WidgetModes mode)
	{
		//ImGui::SeparatorText("Entity Editor");

		for (const auto& it : compMeta.data())
		{
			const entt::meta_data& propData = it.second;
			entt::meta_any propInstance, propInstanceCompare;
			propInstanceCompare = propInstance = propData.get(compInstance);
			ReflectProperty(propData, propInstance, entity, mode);

			//Check if the component has been modified. If so, (re)set the values
			if (propInstance != propInstanceCompare)
				propData.set(compInstance, propInstance);
		}

		return true;
	}

	void Scene::ReflectProperty(const entt::meta_data& propData, entt::meta_any& propValues, Entity* entity, Reflection::WidgetModes mode)
	{
		//Get the (display) name of the component
		std::string propDisplayName;
		entt::meta_prop compDisplayNameProp = propData.prop(Reflection::ComponentProperties::DISPLAY_NAME);
		if (compDisplayNameProp)
			propDisplayName = compDisplayNameProp.value() != nullptr ? compDisplayNameProp.value().cast<std::string>() : std::string(propData.type().info().name());
		else propDisplayName = "Undefined display name (" + std::string(propData.type().info().name()) + ")";

		ImGui::PushID(std::string(std::to_string(propValues.type().id()) + propDisplayName).c_str());
		ReflectType(propValues, entity, propDisplayName, mode);
		ImGui::PopID();
	}

	void Scene::ReflectType(entt::meta_any& typeValues, Entity* entity, const std::string& propName, Reflection::WidgetModes mode)
	{
		if (typeValues.type().func(Reflection::HANDLE_WIDGET_TYPE_FUNC))
		{
			typeValues.type().func(Reflection::HANDLE_WIDGET_TYPE_FUNC).invoke<LAG::Entity&, entt::meta_any, const char*, const Reflection::WidgetModes&>(
				entt::meta_handle(), *entity, entt::forward_as_meta(typeValues), propName.c_str(), mode
			);
		}
		else
			ImGui::Text(std::string("No meta inspect function detected for " + propName).c_str());
	}

	void Scene::ComponentLoop(std::function<void(ComponentData& compData)> func)
	{
		for (auto&& [id, type] : entt::resolve())
		{
			ComponentData data;
			data.ID = type.id();
			if (type.prop(Reflection::VariableProperties::DISPLAY_NAME))
				data.displayName = type.prop(Reflection::VariableProperties::DISPLAY_NAME).value().cast<std::string>();

			func(data);
		}
	}
}