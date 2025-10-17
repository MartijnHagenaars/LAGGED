#include "EntityViewer.h"

#include <ImGui/imgui.h>

#include "Core/Engine.h"
#include "ECS/Entity.h"
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

			// TODO: Implement functionality for showing and adding components

			ImGui::EndPopup();
		}

		std::string selectedEntityDisplay = "Entity ID: " + std::to_string(m_SelectedEntityID);
		ImGui::Text(selectedEntityDisplay.c_str());

		Scene* scene = GetEngine().GetScene();
		for (ArchetypeView archIt : scene->Range())
		{
			if (archIt.Contains(m_SelectedEntityID))
			{
				for (ComponentView compIt : archIt.Types())
				{
					const auto& compProps = compIt.Properties();
					bool isHeaderOpen = (!compProps.isHidden && ImGui::CollapsingHeader(compProps.displayName.c_str(), ImGuiTreeNodeFlags_None));
					
					// Context menu when right-clicking on header
					if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
					{
						ImGui::Text("%s (%s)", compProps.displayName.c_str(), compIt.Name().data());
						
						// TODO: Add extra functionality like removing and/or resetting component data

						ImGui::EndPopup();
					}

					// Contents of opened header
					if (isHeaderOpen)
					{
						EditorGui::Indent indent;
						for (const MemberView& varIt : compIt.Members())
						{
							const auto& varProps = varIt.Properties();
							if (!varProps.isHidden)
							{
								varIt.Func(HANDLE_EDITOR_WIDGET_FUNC).Invoke(m_SelectedEntityID, varIt.ToAny(varIt.GetVoid(m_SelectedEntityID)), varIt.Properties().displayName);
							}
						}
					}
				}
			}
		}
	}
}
