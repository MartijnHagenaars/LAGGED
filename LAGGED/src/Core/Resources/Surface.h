#pragma once
#include "Core/Resources/Resource.h"
#include "Platform/Base/Renderer/Buffer.h"
#include <vector>

namespace LAG
{
	class Shader;
	class Entity;
	struct CameraComponent;
	struct TransformComponent;
	struct LightComponent;
	class SurfaceBase : public Resource
	{
	public:
		SurfaceBase() : Resource(HashedString("")) {};
		virtual ~SurfaceBase() {};

		virtual void Render(TransformComponent& transform, Entity* cameraEntity, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights) = 0;

		virtual bool Load() = 0;
		virtual bool Unload() = 0;

		//TODO: Pure virtual functions for generation surface, applying height map/noise map need to be added here

	protected:
		ArrayBuffer m_Buffer;
	};
}

//Include the correct surface header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_Surface.h"
#elif PLATFORM_DIRECTX
#include "Platform/DX12/Renderer/DX12_Surface.h"
#endif