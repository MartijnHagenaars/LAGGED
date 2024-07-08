#include "LiveFileEditor.h"

#include "Utility/Logger.h"
#include "FileIO/FileIO.h"
#include "FileIO/FileWatch.h"

#include "ImGui/imgui.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Shader.h"

namespace LAG
{
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
			ImGui::End();
			return;
		}

		ImGui::Text("Directory:");
		std::string directoryPath = FileIO::GetPath(FileIO::Directory::Shaders);
		const auto& files = FileIO::GetAllFilesInDirectory(FileIO::Directory::Shaders, "");
		ImGui::Indent(16.f);

		for (size_t i = 0; i < files.size(); i++)
		{
			const std::string& path = files[i];
			bool isWatching = FileWatch::IsWatchingFile(path);

			if (ImGui::Checkbox(std::string("##" + std::to_string(i)).c_str(), &isWatching))
				if (isWatching)
				{
					//In the case of a shader...


					FileWatch::Register(path, [](const std::string& file)
						{
							const std::string& noExt = file.substr(0, file.length() - 5);
							const std::string& noType = noExt.substr(0, noExt.find_last_of("."));
							if (GetResourceManager()->GetResource<Shader>(HashedString(noType))->Reload())
								Logger::Info("Successfully reloaded shader: {0}", noType);
							else
								Logger::Error("Failed to reload shader: {0}", noType);
						}
					);
				}
				else
					FileWatch::Remove(path);

			ImGui::SameLine();
			ImGui::Text(path.substr(directoryPath.length(), path.length()).c_str());
		}
		ImGui::Indent(-16.f);
		ImGui::End();
	}
}
