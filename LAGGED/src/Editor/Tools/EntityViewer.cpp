#include "EntityViewer.h"

#include "ECS/Entity.h"
#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"

#include "Core/Engine.h"
#include "ImGui/imgui.h"

#include "ECS/Components/BasicComponents.h"

namespace LAG
{
	EntityViewer::EntityViewer() :
		ToolBase(ToolType::LEVEL, "Entity Editor", "EntityViewer"), 
		m_SelectedEntityID(ENTITY_NULL),
		m_BrowserHeight(200.f)
	{
	}

	void EntityViewer::Render()
	{
		ImGui::Begin("Entity Editor", &m_IsOpen);

		Scene* scene = GetEngine().GetScene();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::SeparatorText("Entity Editor");

		if (ImGui::Button("Add object"))
		{
			GetScene()->AddEntity(std::string((m_NewEntityName[0] != '\0') ? m_NewEntityName : "Unnamed entity"));
			memset(m_NewEntityName, 0, sizeof(m_NewEntityName));
		}
		ImGui::SameLine();
		ImGui::InputTextWithHint("##InputText", "Enter name here...", m_NewEntityName, sizeof(m_NewEntityName));

		std::string totalEntities = "Total entities: " + std::to_string(scene->Count());
		ImGui::Text(totalEntities.c_str());

		for (const auto& entIt : *scene)
		{
			ImGui::PushID(entIt.ID());

			// FIXME: THIS WILL CAUSE A CRASH IF ENTITY DOESNT HAVE DEFAULTCOMPONENT.
			DefaultComponent* defaultComp = entIt.GetComponent<DefaultComponent>();
			if (ImGui::Button(defaultComp->visible ? "Hide" : "Show"))
				defaultComp->visible = !defaultComp->visible;

			ImGui::SameLine();
			if (ImGui::Button(defaultComp->name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.f)))
			{
				if (m_SelectedEntityID != entIt.ID())
					m_SelectedEntityID = entIt.ID();
			}
			ImGui::PopID();
		}

		ImGui::InvisibleButton("SplitButton", ImVec2(-1.f, 10.0f));
		if (ImGui::IsItemActive())
			m_BrowserHeight += ImGui::GetIO().MouseDelta.y;


		//Selected entity container
		ImGui::BeginChild("EntityProperties", ImVec2(0.f, 0.f), ImGuiChildFlags_Border, ImGuiWindowFlags_None);

		RenderProperties();

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EntityViewer::RenderProperties()
	{
		if(m_SelectedEntityID != ENTITY_NULL)
		{
			if (ImGui::Button("Duplicate Entity"))
			{
				GetScene()->DuplicateEntity(m_SelectedEntityID);
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete Entity"))
			{
				GetScene()->RemoveEntity(m_SelectedEntityID);
				m_SelectedEntityID = ENTITY_NULL;
				return;
			}

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponentPopup");

			if (ImGui::BeginPopup("AddComponentPopup"))
			{
				ImGui::SeparatorText("Select a component");

				ImGui::Text("TODO: ADD LIST OF COMPONENTS HERE...");

				ImGui::EndPopup();
			}

			std::string selectedEntityDisplay = "Entity ID: " + std::to_string(m_SelectedEntityID);
			ImGui::Text(selectedEntityDisplay.c_str());

			ImGui::Text("Add new component by picking one from the list below.");

			//TODO: ADD FUNCTIONALITY FOR DRAWING ENTITIES AND THEIR COMPONENTS
		}
		else ImGui::Text("Select an entity to view its properties");
	}
}
