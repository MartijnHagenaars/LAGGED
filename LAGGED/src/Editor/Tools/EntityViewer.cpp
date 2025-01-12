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
		m_SelectedEntity(nullptr),
		m_BrowserHeight(200.f)
	{
	}

	EntityViewer::~EntityViewer()
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

		//Browser container
		ImGui::BeginChild("EntityList", ImVec2(0.f, m_BrowserHeight), ImGuiChildFlags_Border, ImGuiWindowFlags_None);
		scene->Loop<DefaultComponent>([&scene, &entity = m_SelectedEntity](Entity entity, DefaultComponent& comp)
			{
				ImGui::PushID(entity.GetEntityID().id);
				if (ImGui::Button(comp.visible ? "Hide" : "Show"))
					comp.visible = !comp.visible;

				ImGui::SameLine();
				if (ImGui::Button(comp.name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.f)))
				{
					if (entity.GetEntityID() != entity.GetEntityID())
					{
						//TODO: Implement reflection widgets...
						ImGui::Text("TODO: IMPLEMENT REFLECTION WIDGETS...");
					}
				}
				ImGui::PopID();
			});

		ImGui::EndChild();

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
		if (m_SelectedEntity->IsValid())
		{
			if (ImGui::Button("Duplicate Entity"))
			{
				GetScene()->DuplicateEntity(m_SelectedEntity->GetEntityID());
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete Entity"))
			{
				GetScene()->RemoveEntity(m_SelectedEntity->GetEntityID());
				m_SelectedEntity = nullptr;
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

			std::string selectedEntityDisplay = "Entity ID: " + std::to_string(m_SelectedEntity->GetEntityID().id);
			ImGui::Text(selectedEntityDisplay.c_str());

			ImGui::Text("Add new component by picking one from the list below.");

			//TODO: ADD FUNCTIONALITY FOR DRAWING ENTITIES AND THEIR COMPONENTS
		}
		else ImGui::Text("Select an entity to view its properties");
	}
}
