#pragma once
#include "Core/Resources/Resource.h"


namespace LAG
{
	class Shader;
	struct CameraComponent;
	struct TransformComponent;
	struct LightComponent;
	class SurfaceBase : public Resource
	{
	public:
		SurfaceBase() : Resource(HashedString("")) {};
		virtual ~SurfaceBase() {};

		virtual void Render(TransformComponent& transform, uint32_t cameraEntityID, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights) = 0;

	protected:
		//TODO: Move surface size variables here

	private:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;
	};
}

//Include the correct surface header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_Surface.h"
#elif PLATFORM_DIRECTX
#include "Platform/DX12/Renderer/DX12_Surface.h"
#endif