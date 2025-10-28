#include "EntityViewer.h"

#include <ImGui/imgui.h>

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"

#include "Editor/UI/EditorGuiHelper.h"
#include "Editor/Reflection/EditorTypesReflectionWidgets.h"

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

		EditorGui::SeparatorText("Entity list");
		if (ImGui::Button("Add object"))
		{
			GetScene()->AddEntity(std::string((m_NewEntityName[0] != '\0') ? m_NewEntityName : "Unnamed entity"));
			memset(m_NewEntityName, 0, sizeof(m_NewEntityName));
		}

		ImGui::SameLine(); ImGui::InputTextWithHint("##InputText", "Enter name here...", m_NewEntityName, sizeof(m_NewEntityName));
		ImGui::Text("Total entities: %i", scene->Count());

		if (ImGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY))
		{
			scene->ForEach<>([&scene, &selId = m_SelectedEntityID](EntityID id)
				{
					ImGui::PushID(id);

					// FIXME: THIS WILL CAUSE A CRASH IF ENTITY DOESNT HAVE DEFAULTCOMPONENT.
					DefaultComponent* defaultComp = scene->GetComponent<DefaultComponent>(id);
					if (ImGui::Button(defaultComp->visible ? "Hide" : "Show"))
						defaultComp->visible = !defaultComp->visible;
					ImGui::SameLine();
					if (ImGui::Button(defaultComp->name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.f)))
					{
						if (selId != id)
							selId = id;
					}
					ImGui::PopID();
				}
			);
			ImGui::EndChild();
		}

		RenderProperties();
	}

	void EntityViewer::RenderProperties()
	{
		if (m_SelectedEntityID == ENTITY_NULL)
		{
			EditorGui::SeparatorText("No Entity Selected");
			ImGui::Text("Select an entity from the entity list to view its properties.");
			return;
		}

		Scene* scene = GetEngine().GetScene();
		EditorGui::SeparatorText((std::string("Selected Entity: ") + scene->GetComponent<DefaultComponent>(m_SelectedEntityID)->name).c_str());

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

			//for ()
			//{
			//	ImGui::Button();
			//}
			// TODO: Implement functionality for showing and adding components

			ImGui::EndPopup();
		}

		std::string selectedEntityDisplay = "Entity ID: " + std::to_string(m_SelectedEntityID);
		ImGui::Text(selectedEntityDisplay.c_str());

		for (ArchetypeView archIt : scene->Range())
		{
			if (archIt.Contains(m_SelectedEntityID))
			{
				for (ComponentView compIt : archIt.Types())
				{
					const auto& compProps = compIt.Properties();
					if (compProps.isHidden)
						continue;
					
					// Context menu when right-clicking on header
					std::string_view displayName = !compProps.displayName.empty() ? compProps.displayName : compIt.Name();
					bool isHeaderOpen = ImGui::CollapsingHeader(displayName.data(), ImGuiTreeNodeFlags_None);
					if (ImGui::BeginPopupContextItem())
					{
						ImGui::Text("%s (%s)", displayName.data(), compIt.Name().data());
						
						if(ImGui::Button("Remove Component"))
							compIt.Func(REMOVE_COMPONENT).Invoke(m_SelectedEntityID);
						
						ImGui::EndPopup();
					}

					// Contents of opened header
					if (isHeaderOpen)
					{
						EditorGui::Indent indent;
						if (compIt.Members().begin() == compIt.Members().end())
						{
							ImGui::Text("No reflection data for %s.", displayName.data());
							continue;
						}
						for (const MemberView& varIt : compIt.Members())
						{
							const auto& varProps = varIt.Properties();
							if (!varProps.isHidden)
							{
								varIt.Func(EDITOR_DRAW_TYPE_WIDGET).Invoke(m_SelectedEntityID, varIt.ToAny(varIt.GetVoid(m_SelectedEntityID)), varIt.Properties().displayName);
							}
						}
					}
				}
			}
		}
	}
}
