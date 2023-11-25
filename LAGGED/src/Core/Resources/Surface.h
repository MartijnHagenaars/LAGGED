#pragma once
#include "Core/Resources/Resource.h"


namespace LAG
{
	class Shader;
	struct CameraComponent;
	struct TransformComponent;
	class SurfaceBase : public Resource
	{
	public:
		SurfaceBase() : Resource(HashedString("")) {};
		virtual ~SurfaceBase() {};

		virtual void Render(TransformComponent& transform, uint32_t cameraEntityID, Shader& shader) = 0;

	protected:
		int m_XVertexResolution = 1;
		int m_ZVertexResolution = 1;

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