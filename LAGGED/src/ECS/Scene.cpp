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

						//Only reflect the component if it has any reflected variables set up.
						if (compMeta.data().end() - compMeta.data().begin() > 0)
						{
							if (ImGui::CollapsingHeader(compName.c_str(), ImGuiTreeNodeFlags_None))
							{
								//Apply some nice formatting
								const float spacingAmount = 16.f;
								ImGui::PushID(compMeta.info().hash());
								ImGui::Indent(spacingAmount);

								//Now actually reflect the component
								ReflectComponent(compMeta, compInstance, entity, mode);

								//Reset formatting
								ImGui::Unindent(spacingAmount);
								ImGui::Dummy(ImVec2(spacingAmount, 0.f));
								ImGui::PopID();
							}
						}
					}
					else
					{
						ImGui::SeparatorText(std::string(compMeta.info().name()).c_str());
						ImGui::Text("Failed to load reflection data for component. Has the component reflection been set up?");
					}
				}
			}
		}
	}

	bool Scene::ReflectComponent(entt::meta_type& compMeta, entt::meta_any& compInstance, Entity* entity, Reflection::WidgetModes mode)
	{
		for (const auto& it : compMeta.data())
		{
			const entt::meta_data& propData = it.second;
			entt::meta_any propInstance, propInstanceCompare;
			propInstanceCompare = propInstance = propData.get(compInstance);
			ReflectMember(propData, propInstance, entity, mode);

			//Check if the component has been modified. If so, (re)set the values
			if (propInstance != propInstanceCompare)
				propData.set(compInstance, propInstance);
		}

		return true;
	}

	void Scene::ReflectMember(const entt::meta_data& propData, entt::meta_any& propValues, Entity* entity, Reflection::WidgetModes mode)
	{
		//Check if the member needs to be hidden. If that's the case, we can return early.
		entt::meta_prop hideMember = propData.prop(Reflection::VariableProperties::HIDE_IN_EDITOR);
		if (hideMember && hideMember.value().cast<bool>() == true)
			return;

		//Get the (display) name of the member. If the member doesn't have a display name, use whatever is generated.
		std::string memberDisplayName;
		entt::meta_prop memberDisplayNameProp = propData.prop(Reflection::ComponentProperties::DISPLAY_NAME);
		if (memberDisplayNameProp)
			memberDisplayName = memberDisplayNameProp.value() != nullptr ? memberDisplayNameProp.value().cast<std::string>() : std::string(propData.type().info().name());
		else memberDisplayName = "Undefined display name (" + std::string(propData.type().info().name()) + ")";

		RenderMemberWidget(propValues, entity, memberDisplayName, mode);
	}

	void Scene::RenderMemberWidget(entt::meta_any& typeValues, Entity* entity, const std::string& propName, Reflection::WidgetModes mode)
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

	void Scene::ComponentLoop(std::function<void(Component& comp)> func)
	{
		for (auto&& [id, type] : entt::resolve())
		{
			if (type && type.prop(entt::hashed_string("REFLECTED_COMPONENT")))
			{
				Component component(type);
				func(component);
			}
		}
	}
}