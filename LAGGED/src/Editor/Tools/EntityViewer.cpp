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

					// FIXME: THIS WILL CAUSE A CRASH IF ENTITY DOESNT HAVE EDITOR COMPONENT.
					EditorComponent* editorComp = scene->GetComponent<EditorComponent>(id);
					if (ImGui::Button(editorComp->visible ? "Hide" : "Show"))
						editorComp->visible = !editorComp->visible;
					ImGui::SameLine();
					if (ImGui::Button(editorComp->name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.f)))
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
		EditorGui::SeparatorText((std::string("Selected Entity: ") + scene->GetComponent<EditorComponent>(m_SelectedEntityID)->name).c_str());

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

			const auto& compIDs = scene->QueryCompIDs();
			for (int i = 0; i < compIDs.size(); i++)
			{
				TypeID compID = compIDs[i];
				const auto* compProps = SceneReflect::GetComponentProps(compID);
				if (scene->HasComponent(m_SelectedEntityID, compID) || !compProps || compProps->isHidden)
					continue;

				if (ImGui::Button(compProps->displayName.c_str()))
					scene->AddComponent(m_SelectedEntityID, compID);
			}

			ImGui::EndPopup();
		}

		std::string selectedEntityDisplay = "Entity ID: " + std::to_string(m_SelectedEntityID);
		ImGui::Text(selectedEntityDisplay.c_str());

		TypeID compToDelete = 0;
		for (ArchetypeView archIt : scene->Range())
		{
			if (archIt.Contains(m_SelectedEntityID))
			{
				for (ComponentView compIt : archIt.Types())
				{
					const auto* compProps = compIt.Props();
					if (!compProps || compProps->isHidden)
						continue;
					
					// Context menu when right-clicking on header
					std::string_view displayName = !compProps->displayName.empty() ? compProps->displayName : compIt.Name();
					bool isHeaderOpen = ImGui::CollapsingHeader(displayName.data(), ImGuiTreeNodeFlags_None);
					if (ImGui::BeginPopupContextItem())
					{
						ImGui::Text("%s (%s)", displayName.data(), compIt.Name().data());
						
						if (ImGui::Button("Remove Component"))
							compToDelete = compIt.ID();
						
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
							const auto& varProps = varIt.Props();
							if (!varProps.isHidden)
							{
								auto func = varIt.Func(EDITOR_DRAW_TYPE_WIDGET);
								if (func.Valid())
									func.Invoke(m_SelectedEntityID, varIt.ToAny(varIt.GetVoid(m_SelectedEntityID)), varIt.Props().displayName);
							}
						}
					}
				}
			}
		}

		// HACK: Quick workaround for crash when removing component WITHIN archetype loop
		if(compToDelete != 0)
			scene->RemoveComponent(m_SelectedEntityID, compToDelete);
	}
}
