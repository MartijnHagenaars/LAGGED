#pragma once
#include "Tools/ToolBase.h"

namespace Editor
{
	class LiveFileEditor : public ToolBase
	{
	public:
		LiveFileEditor();

		void Render() override;
	};
}

