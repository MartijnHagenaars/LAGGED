#pragma once
#include "Shaders/OpenGL/ShaderDataBase.h"
#include "glm/glm.hpp"

namespace LAG
{
	class Shader
	{
	public:
		Shader() = delete;
		explicit Shader(std::string shaderPath);
		explicit Shader(const ShaderData::ShaderDataBase& shader);

		void Bind();
		void Unbind();

		void SetBool(const std::string& location, bool value);
		void SetInt(const std::string& location, int value);
		void SetFloat(const std::string& location, float value);
		void SetVec3(const std::string& location, glm::vec3 value); //TODO: Apply GLM variable

	private:
		std::string ReadFile(const std::string& filePath);
		unsigned int CompileShader(const std::string& shaderSource, unsigned int shaderType); 
		unsigned int MakeProgram();
		void CleanUpCompiledShaders();

		unsigned int m_ProgramID = 0; 

		std::string m_VertexSource = "";
		std::string m_PixelSource = "";

		unsigned int m_VertexID = 0;
		unsigned int m_PixelID = 0;

	};

}
