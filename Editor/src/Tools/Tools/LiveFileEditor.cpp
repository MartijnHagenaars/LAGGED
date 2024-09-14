#include "LiveFileEditor.h"

#include "Utility/Logger.h"
#include "FileIO/FileIO.h"
#include "FileIO/FileWatch.h"

#include "ImGui/imgui.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Shader.h"
#include "Core/Resources/Model.h"

namespace Editor
{
	//Forward declaring a function that'll be used for reusing common ImGui rendering code
	void RenderDirectories(LAG::FileIO::Directory dir, const std::string& displayName, std::function<void(const std::string&)> callbackFunction, const std::string& relPath = "");
	void RenderDirectoryFiles(LAG::FileIO::Directory dir, const std::string& relPath, std::function<void(const std::string&)> callbackFunction);

	LiveFileEditor::LiveFileEditor() :
		ToolBase(ToolType::GRAPHICS, "Live File Editor", "LiveFileEditor")
	{
	}

	void LiveFileEditor::Render()
	{
		ImGui::Begin(GetDisplayName().c_str(), &m_IsOpen);

		bool isEnabled = LAG::FileWatch::IsEnabled();
		if (ImGui::Checkbox("Enable file watch", &isEnabled))
			LAG::FileWatch::EnableFileWatch(isEnabled);

		ImGui::Separator();
		if (!isEnabled)
		{
			ImGui::Text("Turn on the file watch to enable live editing of files.");
			ImGui::End();
			return;
		}

		//Render shader directory
		RenderDirectories(LAG::FileIO::Directory::Shaders, "Shaders", [](const std::string& file)
			{
				const std::string& noExt = file.substr(0, file.length() - 5);
				const std::string& noType = noExt.substr(0, noExt.find_last_of("."));
				if (LAG::GetResourceManager()->GetResource<LAG::Shader>(LAG::HashedString(noType))->Reload())
					INFO("Successfully reloaded shader: {0}", noType);
				else
					ERROR("Failed to reload shader: {0}", noType);
			});

		//Render model directory
		RenderDirectories(LAG::FileIO::Directory::Models, "Models", [](const std::string& file)
			{
				if (LAG::GetResourceManager()->GetResource<LAG::Model>(LAG::HashedString(file))->Reload())
					INFO("Successfully reloaded shader: {0}", file);
				else
					ERROR("Failed to reload shader: {0}", file);
			});

		ImGui::End();
	}

	void RenderDirectories(LAG::FileIO::Directory dir, const std::string& displayName, std::function<void(const std::string&)> callbackFunction, const std::string& relPath)
	{
		if (relPath.length() == 0)
			ImGui::SeparatorText(displayName.c_str());

		const auto& dirs = LAG::FileIO::GetAllSubDirectories(dir, relPath);
		for (const auto& it : dirs)
		{
			ImGui::Indent(16.f);
			if (ImGui::TreeNode(it.c_str()))
			{
				RenderDirectories(dir, displayName, callbackFunction, it);
				ImGui::TreePop();
			}
			ImGui::Indent(-16.f);
		}

		RenderDirectoryFiles(dir, relPath, callbackFunction);
	}

	void RenderDirectoryFiles(LAG::FileIO::Directory dir, const std::string& relPath, std::function<void(const std::string&)> callbackFunction)
	{
		const auto& files = LAG::FileIO::GetAllFilesInDirectory(dir, relPath);
		ImGui::Indent(16.f);

		//Loop through all files in the directory
		for (const auto& path : files)
		{
			bool isWatching = LAG::FileWatch::IsWatchingFile(dir, path);

			//Checkbox widget for registering/removing file
			if (ImGui::Checkbox(std::string("##" + path).c_str(), &isWatching))
				if (isWatching)
				{
					LAG::FileWatch::Register(dir, path, callbackFunction);
				}
				else
					LAG::FileWatch::Remove(dir, path);

			ImGui::SameLine();
			ImGui::Text(path.c_str());
		}
		ImGui::Indent(-16.f);
	}
}
