#pragma once
#include "Core/Defines.h"
#include "WindowBase.h"

namespace LAG::Renderer
{
	// ===== Essential render functions =====

	LAG_API bool Initialize();
	LAG_API bool Shutdown();

	LAG_API void StartFrame();
	LAG_API void EndFrame();

	LAG_API void Render();
	LAG_API void Clear();

	void OnResize();

	LAG_API void SetPipelineState(int pipelineID);

}
