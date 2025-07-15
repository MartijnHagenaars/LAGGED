#pragma once
#include "Editor/ToolBase.h"
#include <vector>

namespace LAG
{
	class ProfilerViewer : public ToolBase
	{
	public:
		ProfilerViewer();

		void Render() override;
	};
}
