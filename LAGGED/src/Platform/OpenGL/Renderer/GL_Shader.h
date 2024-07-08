#pragma once
#include "Core/Resources/Shader.h"

namespace LAG
{
	class Shader : public ShaderBase
	{
		friend class ResourceManager;
	public:
		Shader() = delete;
		explicit Shader(const HashedString& path);
		~Shader() override;

		void Bind() override;
		void Unbind() override;

		void SetBool(const std::string& location, bool value) override;
		void SetInt(const std::string& location, int value) override;
		void SetFloat(const std::string& location, float value) override;
		void SetVec3(const std::string& location, glm::vec3 value) override;
		void SetMat4(const std::string& location, const glm::mat4& value) override;

		bool Reload() override;
	private:
		bool Load() override;
		bool Unload() override;

		std::string ReadFile(const std::string& filePath); //TODO: Should be moved. A FileIO class should take care of this. 
		unsigned int CompileShader(const std::string& shaderSource, unsigned int shaderType); 
		unsigned int MakeProgram();
		void CleanUpCompiledShaders();

		unsigned int m_ProgramID = 0; 

		unsigned int m_VertexID = 0;
		unsigned int m_PixelID = 0;
	};

}
