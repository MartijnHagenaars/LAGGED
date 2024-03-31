#pragma once
#include "Core/Defines.h"

namespace LAG::Renderer
{
	// ===== Essential render functions =====

	bool Initialize();
	bool Shutdown();

	void Render();
	void Clear();

	void OnResize(unsigned int width, unsigned int height);

	void SetPipelineState(int pipelineID);

}
