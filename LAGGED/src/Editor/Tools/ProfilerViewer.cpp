#include "ProfilerViewer.h"
#include "Utility/Profiler.h"

#include <ImGui/imgui.h>

namespace LAG
{
	ProfilerViewer::ProfilerViewer() :
		ToolBase(ToolType::PERFORMANCE, "Profiler", "Profiler")
	{
	}

	void ProfilerViewer::Render()
	{
		Profiler::CalculateAverages();

		if (ImGui::BeginTable("records", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
		{
			// Display headers so we can inspect their interaction with borders.
			// (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)

			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Avg. time");
			ImGui::TableSetupColumn("Samples");
			ImGui::TableHeadersRow();

			float accumAvg = 0.f;
			const auto& records = Profiler::GetRecords();
			for (auto& it : records)
			{
				const Profiler::ProfilerRecord & rec = it.second;
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", it.first.c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%fms", rec.average);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%i", rec.history.size());

				accumAvg += rec.average;
			}
			ImGui::EndTable();
		}
	}
}