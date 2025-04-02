#pragma once
#include <vector>

#include "Core/Resources/Resource.h"
#include "Platform/Resources/Buffer/Buffer.h"

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

#endif