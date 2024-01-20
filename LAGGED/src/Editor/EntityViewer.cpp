#include "Precomp.h"
#include "EntityViewer.h"

#include "ECS/Entity.h"
#include "ECS/Scene.h"
#include "ECS/Meta/MetaDefines.h"
#include "ECS/Components/BasicComponents.h"

#include "Core/Engine.h"
#include "ImGui/imgui.h"

namespace LAG
{
	EntityViewer::EntityViewer() :
		m_BrowserHeight(200.f)
	{
	}

	EntityViewer::~EntityViewer()
	{
	}

	void EntityViewer::Update()
	{
	}


	void ReflectType()
	{

	}

	void ReflectProperty(entt::meta_data& propData, entt::meta_any& propValues, entt::entity entityID)
	{
		std::string propDisplayName;
		entt::meta_prop displayNameProp = propData.prop(Reflection::DISPLAY_NAME);
		if (displayNameProp.value() != nullptr)
			propDisplayName = displayNameProp.value().cast<std::string>();
		else
			propDisplayName = "Undefined property display name";

		ImGui::Text(propDisplayName.c_str());
	}

	bool ReflectComponent(entt::meta_type& compMeta, entt::sparse_set& storageSet, entt::entity entityID)
	{
		std::string compName = std::string(compMeta.info().name());
		ImGui::Text(compName.c_str());

		entt::meta_any compElement = compMeta.from_void(storageSet.value(entt::entity(entityID)));
		if (compElement == nullptr)
			return false;

		for (auto&& [idType, propMetaData] : compMeta.data())
		{
			entt::meta_any propInstance = propMetaData.get(compElement);
			ReflectProperty(propMetaData, propInstance, entityID);
		}

		return true;
	}

	void EntityViewer::Render()
	{
		ImGui::Begin("Entity Viewer");

		Scene* scene = GetEngine().GetScene();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::SeparatorText("Entity Viewer");

		std::string totalEntities = "Total entities: " + std::to_string(scene->Count());
		ImGui::Text(totalEntities.c_str());
		
		//Browser container
		ImGui::BeginChild("EntityList", ImVec2(0.f, m_BrowserHeight), ImGuiChildFlags_Border, ImGuiWindowFlags_None);
		scene->Loop<DefaultComponent>([&scene, &selectedEntityID = m_SelectedEntityID](uint32_t entityID, DefaultComponent& comp)
			{
				ImGui::PushID(entityID);
				if (ImGui::Button(comp.visible ? "Hide" : "Show"))
					comp.visible = !comp.visible;

				ImGui::SameLine();
				if (ImGui::Button(comp.name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.f)))
					selectedEntityID = entityID;
				ImGui::PopID();
			});

		ImGui::EndChild();

		ImGui::InvisibleButton("splitter", ImVec2(-1.f, 10.0f));
		if (ImGui::IsItemActive())
			m_BrowserHeight += ImGui::GetIO().MouseDelta.y;



		//Selected entity container
		ImGui::BeginChild("EntityProperties", ImVec2(0.f, 0.f), ImGuiChildFlags_Border, ImGuiWindowFlags_None);
		std::string selectedEntityDisplay = "Entity ID: " + std::to_string(m_SelectedEntityID);
		ImGui::Text(selectedEntityDisplay.c_str());
		
		for (auto&& [idType, storageSet] : GetScene()->m_Registry.storage())
		{
			if (storageSet.contains(entt::entity(m_SelectedEntityID)))
			{
				auto componentMeta = entt::resolve(storageSet.type());
				if (!componentMeta)
					continue;

				if (!ReflectComponent(componentMeta, storageSet, entt::entity(m_SelectedEntityID)))
					ImGui::Text("Failed to load reflection data for component.");
				else ImGui::Separator();

			}
		}

		ImGui::EndChild();

		ImGui::PopStyleVar();

		ImGui::End();

	}
}
