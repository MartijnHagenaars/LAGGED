#include "EntityViewer.h"

#include "ECS/Entity.h"
#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionDefines.h"
#include "ECS/Components/BasicComponents.h"

#include "Core/Engine.h"
#include "ImGui/imgui.h"

//For testing
#include "ECS/Components/CameraComponent.h"

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
		scene->Loop<DefaultComponent>([&scene, &selectedEntity = m_SelectedEntity](Entity entity, DefaultComponent& comp)
			{
				ImGui::PushID(entity.GetEntityID());
				if (ImGui::Button(comp.visible ? "Hide" : "Show"))
					comp.visible = !comp.visible;

				ImGui::SameLine();
				if (ImGui::Button(comp.name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.f)))
				{
					//Unload the previously selected entity (if it's valid)
					if (selectedEntity.IsValid())
						GetScene()->HandleComponentWidgets(&selectedEntity, Reflection::WidgetModes::UNLOAD);

					//Set new selected entity and load its widgets
					selectedEntity = entity;
					GetScene()->HandleComponentWidgets(&selectedEntity, Reflection::WidgetModes::LOAD);
				}
				ImGui::PopID();
			});

		ImGui::EndChild();

		ImGui::InvisibleButton("SplitButton", ImVec2(-1.f, 10.0f));
		if (ImGui::IsItemActive())
			m_BrowserHeight += ImGui::GetIO().MouseDelta.y;


		//Selected entity container
		ImGui::BeginChild("EntityProperties", ImVec2(0.f, 0.f), ImGuiChildFlags_Border, ImGuiWindowFlags_None);

		if (m_SelectedEntity.IsValid())
		{
			std::string selectedEntityDisplay = "Entity ID: " + std::to_string(m_SelectedEntity.GetEntityID());
			ImGui::Text(selectedEntityDisplay.c_str());

			//Draw all component widgets
			GetScene()->HandleComponentWidgets(&m_SelectedEntity, Reflection::WidgetModes::DRAW);
		}
		else ImGui::Text("Select an entity to view its properties");

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::End();
	}
}
