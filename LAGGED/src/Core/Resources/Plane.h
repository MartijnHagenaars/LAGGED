#pragma once
#include "Core/Resources/Resource.h"


namespace LAG
{
	class Shader;
	struct CameraComponent;
	struct TransformComponent;
	class PlaneBase : public Resource
	{
	public:
		PlaneBase() = delete;
		explicit PlaneBase(const HashedString& path) : Resource(path) {};
		virtual ~PlaneBase() {};

		virtual void Render(TransformComponent& transform, uint32_t cameraEntityID, Shader& shader) = 0;

	private:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;
	};
}

//Include the correct model header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_Plane.h"
#elif PLATFORM_DIRECTX
#include "Platform/DX12/Renderer/DX12_Plane.h"
#endif