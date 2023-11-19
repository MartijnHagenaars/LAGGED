#pragma once
#include "Core/Resources/Resource.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

enum class ShaderType
{
	VERTEX = 0,
	PIXEL, 
	TESSELLATION

};

namespace LAG
{
	class ShaderBase : public Resource
	{
	public: 
		ShaderBase() = delete;
		explicit ShaderBase(const HashedString& path) : Resource(path) {};
		virtual ~ShaderBase() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetBool(const std::string& location, bool value) = 0;
		virtual void SetInt(const std::string& location, int value) = 0;
		virtual void SetFloat(const std::string& location, float value) = 0;
		virtual void SetVec3(const std::string& location, glm::vec3 value) = 0;
		virtual void SetMat4(const std::string& location, const glm::mat4& value) = 0;

	protected:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;
	};
}

//Include the correct model header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_Shader.h"
#elif PLATFORM_DIRECTX
#include "Platform/DX12/Renderer/DX12_Shader.h"
#endif