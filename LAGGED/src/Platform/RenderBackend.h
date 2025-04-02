#pragma once
#include "Core/Defines.h"
#include "Resources/Model.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

namespace LAG
{
	ErrResult Initialize();
	ErrResult Shutdown();



	Model* GetModel();
	Shader* GetShader();
	Texture* GetTexture();
}