#include "ProfilerViewer.h"
#include "Utility/Profiler.h"

namespace LAG
{
	ProfilerViewer::ProfilerViewer() :
		ToolBase(ToolType::PERFORMANCE, "Profiler", "Profiler")
	{
	}

	void ProfilerViewer::Render()
	{
		Profiler::OnImGui();
	}
}