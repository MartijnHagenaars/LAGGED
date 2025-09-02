#include "EntityViewer.h"

#include "ECS/Entity.h"
#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"

#include "Core/Engine.h"

#include <ImGui/imgui.h>
#include "Editor/UI/EditorGuiHelper.h"

#include "ECS/Components/BasicComponents.h"

namespace LAG
{
	EntityViewer::EntityViewer() :
		ToolBase(ToolType::LEVEL, "Entity Editor", "EntView"), 
		m_SelectedEntityID(ENTITY_NULL)
	{
		memset(m_NewEntityName, 0, sizeof(m_NewEntityName));
	}

	void EntityViewer::Render()
	{
		Scene* scene = GetEngine().GetScene();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		EditorGui::SeparatorText("Entity list");

		if (ImGui::Button("Add object"))
		{
			GetScene()->AddEntity(std::string((m_NewEntityName[0] != '\0') ? m_NewEntityName : "Unnamed entity"));
			memset(m_NewEntityName, 0, sizeof(m_NewEntityName));
		}
		ImGui::SameLine();
		ImGui::InputTextWithHint("##InputText", "Enter name here...", m_NewEntityName, sizeof(m_NewEntityName));

		std::string totalEntities = "Total entities: " + std::to_string(scene->Count());
		ImGui::Text(totalEntities.c_str());

		if (ImGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
			//for (const auto& entIt : *scene)
			//{
			//	ImGui::PushID(entIt.ID());

			//	// FIXME: THIS WILL CAUSE A CRASH IF ENTITY DOESNT HAVE DEFAULTCOMPONENT.
			//	DefaultComponent* defaultComp = entIt.GetComponent<DefaultComponent>();
			//	if (ImGui::Button(defaultComp->visible ? "Hide" : "Show"))
			//		defaultComp->visible = !defaultComp->visible;

			//	ImGui::SameLine();
			//	if (ImGui::Button(defaultComp->name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.f)))
			//	{
			//		if (m_SelectedEntityID != entIt.ID())
			//			m_SelectedEntityID = entIt.ID();
			//	}
			//	ImGui::PopID();
			//}

			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::EndChild();
		}

		RenderProperties();

		ImGui::PopStyleVar();
	}

	void EntityViewer::RenderProperties()
	{
		if (m_SelectedEntityID == ENTITY_NULL)
		{
			EditorGui::SeparatorText("No Entity Selected");
			ImGui::Text("Select an entity from the entity list to view its properties.");
			return;
		}

		Entity entity = GetScene()->GetEntity(m_SelectedEntityID);

		EditorGui::SeparatorText((std::string("Selected Entity: ") + entity.GetComponent<DefaultComponent>()->name).c_str());

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
			for (auto it : SceneReflect::Get())
			{
				if (ImGui::Button(it->props.displayName.c_str()))
				{
					INFO("Add {} to Entity {}", it->props.displayName, m_SelectedEntityID);
				}
			}
			ImGui::EndPopup();
		}

		std::string selectedEntityDisplay = "Entity ID: " + std::to_string(m_SelectedEntityID);
		ImGui::Text(selectedEntityDisplay.c_str());

		for (auto varIt : entity)
		{
			const auto& properties = varIt->props;
			if (properties.isHidden)
			{
				continue;
			}


			if (ImGui::CollapsingHeader(properties.displayName.c_str(), ImGuiTreeNodeFlags_None))
			{
				EditorGui::Indent indent;

				ImGui::Text("Hello there!");
			}
		}
	}
}
