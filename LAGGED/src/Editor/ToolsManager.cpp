#include "ToolsManager.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <json/json.hpp>

#include "Core/IO/FileIO.h"
#include "Core/Engine.h"
#include "Platform/Window.h"
#include "ECS/Scene.h"
#include "ECS/Systems/CameraSystem.h"

#include "UI/Gizmo.h"

#include "Utility/Hash.h"

#include "Tools/CameraViewport.h"
#include "Tools/EntityViewer.h"
#include "Tools/GizmoViewer.h"
#include "Tools/ImGuiTools.h"
#include "Tools/LiveFileEditor.h"
#include "Tools/ProfilerViewer.h"

//TODO: Consider moving this somewhere else..
#define EDITOR_CONFIG_PATH "EditorConfig.cfg"

#define EDITOR_CONFIG_NODE_ID "NodeID"
#define EDITOR_CONFIG_NODE_SPLIT_AXIS "SplitAxis"
#define EDITOR_CONFIG_NODE_SPLIT_RATIO "SplitRatio"
#define EDITOR_CONFIG_NODE_CHILDREN "Children"
#define EDITOR_CONFIG_NODE_WINDOWS "Windows"

static ImGuiID s_DockspaceID = 0;

namespace LAG
{
	void ToolsManager::Initialize()
	{
		m_Gizmo = std::make_unique<Gizmo>();

		RegisterTool<EntityViewer>();
		RegisterTool<CameraViewport>();
		RegisterTool<GizmoViewer>(*m_Gizmo);

		RegisterTool<LiveFileEditor>();
		RegisterTool<ImGuiDemoViewer>();
		RegisterTool<ImGuiStyleEditor>();

		RegisterTool<ProfilerViewer>();
	}

	void ToolsManager::Shutdown()
	{
		SaveToolLayout();
		for (auto& [key, tool] : m_Tools)
		{
			tool.reset();
			tool = nullptr;
		}
	}

	void ToolsManager::PresentEditor()
	{
		BeginDockSpace();

		// Create the menu bar, displayed at the top of the window
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
					GetWindow()->Close();

				ImGui::EndMenu();
			}

			// Add tool categories to menu bar
			for (int catId = 0; catId < s_ToolCategoryStrings.size(); catId++)
			{
				if (ImGui::BeginMenu(s_ToolCategoryStrings[catId]))
				{
					for (auto const& [key, tool] : m_Tools)
					{
						if (tool->Category() != static_cast<ToolCategory>(catId))
							continue;

						if (ImGui::MenuItem(tool->DisplayName().c_str()))
							tool->ToggleTool();
					}
					ImGui::EndMenu();
				}
			}

			ImGui::EndMenuBar();
		}

		// Draw tool windows
		for (auto const& [key, tool] : m_Tools)
		{
			if (tool->IsOpen())
			{
				if (tool->WindowBegin())
					tool->Render();
				ImGui::GetStateStorage()->SetVoidPtr(ImHashStr("ID"), (void*)(key));
				tool->WindowEnd();
			}
		}

		// Update gizmo
		EntityID cameraEntityID = CameraSystem::GetActiveCameraEntityID();
		Gizmo::SetCameraID(CameraSystem::GetActiveCameraEntityID());
		Gizmo::SetTargetID(static_cast<EntityViewer*>(m_Tools.at(GetTypeHash64<EntityViewer>()).get())->GetSelectedEntityID());

		EndDockSpace();
	}

	bool ToolsManager::IsToolOpen(Hash64 toolID)
	{
		if (const auto& it = m_Tools.find(toolID); it != m_Tools.end())
			return it->second->IsOpen();
		else
			CRITICAL("Failed to find tool with ID \"{0}\".", toolID);

		return false;
	}

	void ToolsManager::BeginDockSpace()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);

		ImGuiWindowFlags dockspaceFlags =
			ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings;

		bool isOpen = true;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		if (!ImGui::Begin("Editor", &isOpen, dockspaceFlags))
			CRITICAL("Failed to load dockspace");

		//Create the dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockID = ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
			if (s_DockspaceID == 0)
			{
				s_DockspaceID = dockID;
				LoadToolLayout();
			}
		}

	}

	void ToolsManager::EndDockSpace()
	{
		ImGui::End();
		ImGui::PopStyleVar();
	}

	using ToolsDatabase = std::unordered_map<Hash64, std::unique_ptr<LAG::ToolBase>>;
	void DeserializeDockNode(nlohmann::json& data, const ToolsDatabase& tools, ImVec2 workSize, ImGuiID nodeID)
	{
		ImGui::DockBuilderSetNodeSize(nodeID, workSize);
		if (data.contains(EDITOR_CONFIG_NODE_SPLIT_AXIS))
		{
			ImGuiDir dir = data[EDITOR_CONFIG_NODE_SPLIT_AXIS];
			float ratio = data[EDITOR_CONFIG_NODE_SPLIT_RATIO];

			bool isHorizontal = (dir == ImGuiDir_Left || dir == ImGuiDir_Right);
			ImVec2 sizeDir = ImVec2(isHorizontal ? workSize.x * ratio : workSize.x, !isHorizontal ? workSize.y * ratio : workSize.y);
			ImVec2 sizeOther = ImVec2(isHorizontal ? workSize.x * (1.0f - ratio) : workSize.x, !isHorizontal ? workSize.y * (1.0f - ratio) : workSize.y);

			ImGuiID dirID = 0, oppositeID = 0;
			ImGui::DockBuilderSplitNode(nodeID, dir, ratio, &dirID, &oppositeID);

			DeserializeDockNode(data[EDITOR_CONFIG_NODE_CHILDREN][0], tools, sizeDir, dirID);
			DeserializeDockNode(data[EDITOR_CONFIG_NODE_CHILDREN][1], tools, sizeOther, oppositeID);
		}
		else
		{
			for (auto toolID : data[EDITOR_CONFIG_NODE_WINDOWS])
			{
				ToolBase* tool = tools.at(toolID).get();
				ImGui::DockBuilderDockWindow(tool->DisplayName().c_str(), nodeID);
				tool->Open();
			}
		}
	}

	void ToolsManager::LoadToolLayout()
	{
		if (!FileIO::Exists(FileIO::Directory::Saves, EDITOR_CONFIG_PATH))
		{
			ERROR("Failed to load editor tools layout: File does not exist.");
			return;
		}

		INFO("Loading editor tool layout...");
		nlohmann::json data = nlohmann::json::parse(FileIO::Read(FileIO::Directory::Saves, EDITOR_CONFIG_PATH));

		ImGui::DockBuilderRemoveNode(s_DockspaceID);
		ImGui::DockBuilderAddNode(s_DockspaceID, ImGuiDockNodeFlags_DockSpace);

		ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;
		DeserializeDockNode(data, m_Tools, workSize, s_DockspaceID);

		ImGui::DockBuilderFinish(s_DockspaceID);
	}

	nlohmann::json SerializeDockNode(ImGuiDockNode* node)
	{
		nlohmann::json data = {};
		data[EDITOR_CONFIG_NODE_ID] = node->ID;

		if (node->IsSplitNode())
		{
			ImGuiDockNode* child0 = node->ChildNodes[0];
			ImGuiDockNode* child1 = node->ChildNodes[1];

			float ratio;
			ImGuiDir dir;
			if (node->SplitAxis == ImGuiAxis_X)
			{
				ratio = child0->SizeRef.x / node->SizeRef.x;
				dir = (child0->Pos.x < child1->Pos.x) ? ImGuiDir_Left : ImGuiDir_Right;
			}
			else
			{
				ratio = child0->SizeRef.y / node->SizeRef.y;
				dir = (child0->Pos.y < child1->Pos.y) ? ImGuiDir_Up : ImGuiDir_Down;
			}

			data[EDITOR_CONFIG_NODE_SPLIT_AXIS] = dir;
			data[EDITOR_CONFIG_NODE_SPLIT_RATIO] = ratio;

			data[EDITOR_CONFIG_NODE_CHILDREN] = nlohmann::json::array();
			data[EDITOR_CONFIG_NODE_CHILDREN].push_back(SerializeDockNode(child0));
			data[EDITOR_CONFIG_NODE_CHILDREN].push_back(SerializeDockNode(child1));
		}
		else
		{
			data[EDITOR_CONFIG_NODE_WINDOWS] = nlohmann::json::array();
			for (int i = 0; i < node->TabBar->Tabs.Size; ++i)
			{
				Hash64 toolID = reinterpret_cast<Hash64>(node->TabBar->Tabs[i].Window->StateStorage.GetVoidPtr(ImHashStr("ID")));
				data[EDITOR_CONFIG_NODE_WINDOWS].push_back(toolID);
			}
		}

		return data;
	}

	void ToolsManager::SaveToolLayout()
	{
		ImGuiDockNode* root = ImGui::DockBuilderGetNode(s_DockspaceID);
		if (!root)
			return;

		nlohmann::json nodes = SerializeDockNode(root);
		FileIO::Write(FileIO::Directory::Saves, EDITOR_CONFIG_PATH, nodes.dump(4));
		INFO("Finished saving editor tool layout...");
	}
}
