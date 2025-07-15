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

		float accumAvg = 0.f;
		ImGui::BeginTable("records", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable);
		
		// Setup table
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Avg. time");
		ImGui::TableSetupColumn("Samples");
		ImGui::TableHeadersRow();

		// Populate table with data
		const auto& records = Profiler::GetRecords();
		for (auto& it : records)
		{
			const Profiler::ProfilerRecord& rec = it.second;
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

		ImGui::Text("Total accumulated average: %f", accumAvg);
	}
}