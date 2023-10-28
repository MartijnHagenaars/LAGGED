#pragma once
#include "Core/Defines.h"
#include "Platform/Base/Window/WindowBase.h"

namespace LAG::Renderer
{
	// ===== Essential render functions =====

	bool Initialize();
	bool Shutdown();

	void StartFrame();
	void EndFrame();

	void Render();
	void Clear();

	void OnResize();

	void SetPipelineState(int pipelineID);

}
