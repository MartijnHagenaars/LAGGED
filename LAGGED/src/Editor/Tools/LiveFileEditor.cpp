#include "LiveFileEditor.h"

#include "Utility/Logger.h"
#include "FileIO/FileIO.h"
#include "FileIO/FileWatch.h"

#include "ImGui/imgui.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Shader.h"
#include "Core/Resources/Model.h"

namespace LAG
{
	//Forward declaring a function that'll be used for reusing common ImGui rendering code
	void RenderDirectoryFiles(FileIO::Directory dir, const std::string& dirName, std::function<void(const std::string&)> callbackFunction);

	LiveFileEditor::LiveFileEditor() :
		ToolBase(ToolType::GRAPHICS, "Live File Editor", "LiveFileEditor")
	{
	}

	void LiveFileEditor::Render()
	{
		ImGui::Begin(GetDisplayName().c_str(), &m_IsOpen);

		bool isEnabled = FileWatch::IsEnabled();
		if (ImGui::Checkbox("Enable file watch", &isEnabled))
			FileWatch::EnableFileWatch(isEnabled);

		ImGui::Separator();
		if (!isEnabled)
		{
			ImGui::Text("Turn on the file watch to enable live editing of files.");
			ImGui::End();
			return;
		}
		
		//Render shader directory
		RenderDirectoryFiles(FileIO::Directory::Shaders, "Shaders", [](const std::string& file)
			{
				const std::string& noExt = file.substr(0, file.length() - 5);
				const std::string& noType = noExt.substr(0, noExt.find_last_of("."));
				if (GetResourceManager()->GetResource<Shader>(HashedString(noType))->Reload())
					Logger::Info("Successfully reloaded shader: {0}", noType);
				else
					Logger::Error("Failed to reload shader: {0}", noType);
			});

		//Render model directory
		RenderDirectoryFiles(FileIO::Directory::Models, "Models", [](const std::string& file)
			{
				if (GetResourceManager()->GetResource<Model>(HashedString(file))->Reload())
					Logger::Info("Successfully reloaded shader: {0}", file);
				else
					Logger::Error("Failed to reload shader: {0}", file);
			});
		
		ImGui::End();
	}

	void RenderDirectoryFiles(FileIO::Directory dir, const std::string& dirName, std::function<void(const std::string&)> callbackFunction)
	{
		ImGui::SeparatorText(dirName.c_str());

		std::string directoryPath = FileIO::GetPath(dir);
		const auto& files = FileIO::GetAllFilesInDirectory(dir, "");
		ImGui::Indent(16.f);
		
		//Loop through all files in the directory
		for (size_t i = 0; i < files.size(); i++)
		{
			const std::string& path = files[i];
			bool isWatching = FileWatch::IsWatchingFile(path);

			//Checkbox widget for registering/removing file
			if (ImGui::Checkbox(std::string("##" + std::to_string(i)).c_str(), &isWatching))
				if (isWatching)
				{
					FileWatch::Register(path, callbackFunction);
				}
				else
					FileWatch::Remove(path);

			ImGui::SameLine();
			ImGui::Text(path.substr(directoryPath.length(), path.length()).c_str());
		}
		ImGui::Indent(-16.f);
	}
}
