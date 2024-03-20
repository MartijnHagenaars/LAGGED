#pragma once
#include "Core/Defines.h"
#include "Platform/Base/Window/WindowBase.h"

namespace LAG::Renderer
{
	// ===== Essential render functions =====

	bool Initialize();
	bool Shutdown();

	void Render();
	void Clear();

	void OnResize(unsigned int width, unsigned int height);

	// This is supposed to be the base of the renderer but I don't think OpenGL has any pipeline state objects
	// So why is this part of your base API?
	void SetPipelineState(int pipelineID);

}
