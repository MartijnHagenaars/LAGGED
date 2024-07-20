#pragma once
#include "Tools/ToolBase.h"

namespace LAGEditor
{
	class LiveFileEditor : public ToolBase
	{
	public:
		LiveFileEditor();

		void Render() override;
	};
}

