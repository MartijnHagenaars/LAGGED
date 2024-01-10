#include "Precomp.h"
#include "EntityViewer.h"

#include "ECS/Entity.h"
#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"

#include "Core/Engine.h"
#include "ImGui/imgui.h"

namespace LAG
{
	void EntityViewer::Update()
	{
	}

	void EntityViewer::Render()
	{
		ImGui::Begin("Entity Viewer");

		Scene* scene = GetEngine().GetScene();

		std::string totalEntities = "Total entities: " + std::to_string(scene->Count());
		ImGui::Text(totalEntities.c_str());
		ImGui::Separator();

		scene->Loop<NameComponent>([&scene](uint32_t entityID, NameComponent& name)
			{
				std::string displayName = name.name;
				ImGui::PushID(entityID);
				//TODO: Renaming this currently closes the collapsing header
				if (ImGui::CollapsingHeader(displayName.c_str(), ImGuiTreeNodeFlags_None))
				{
					ImGui::Text("Defaults:");
					
					//Name text field
					if (ImGui::InputText("Name", displayName.data(), 64, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
						if (displayName != name.name.c_str()) //TODO: name.name is very ugly and dumb. Change this!!
							name.name = displayName;

					ImGui::Dummy(ImVec2(0.0f, 8.f));
					ImGui::Separator();
					ImGui::Dummy(ImVec2(0.0f, 8.f));

					ImGui::Text("Other fields go here...");

					ImGui::Dummy(ImVec2(0.0f, 8.f));
				}
				ImGui::PopID();

			});
		
		for (int i = 0; i < GetEngine().GetScene()->Count(); i++)
		{

			//ImGui::
		}

		ImGui::End();
	}
}
