#pragma once
#include <glm/mat4x4.hpp>
#include <string>

#include "Core/Resources/Resource.h"

namespace LAG
{
	class Shader : public Resource
	{
		friend class ResourceManager;
	public:
		Shader() = delete;
		explicit Shader(const StringHash& path);
		~Shader() override;

		void Bind();
		void Unbind();

		void SetBool(const std::string& location, bool value);
		void SetInt(const std::string& location, int value);
		void SetFloat(const std::string& location, float value);
		void SetVec3(const std::string& location, glm::vec3 value);
		void SetMat4(const std::string& location, const glm::mat4& value);

		bool Reload() override;
	private:
		bool Load() override;
		bool Unload() override;

		unsigned int CompileShader(const std::string& shaderSource, unsigned int shaderType); 
		unsigned int MakeProgram();
		
		void CleanUpCompiledShaders();

		void* m_DataPtr = nullptr;
	};

}
