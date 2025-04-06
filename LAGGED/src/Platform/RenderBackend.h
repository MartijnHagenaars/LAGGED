#pragma once
#include "Core/Defines.h"

namespace LAG
{
	ErrResult Initialize();
	ErrResult Shutdown();

	void DrawDebugLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);

	Model* GetModel();
	Shader* GetShader();
	Texture* GetTexture();
}