#include "Precomp.h"
#include "GL_Shader.h"

#include <fstream>
#include <filesystem>

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/Renderer/Exceptions/GL_GraphicsExceptionMacros.h"

namespace LAG
{
	Shader::Shader(const Utility::String& shaderPath) : ShaderBase(shaderPath)
	{
	}

	Shader::~Shader()
	{
	}

	bool Shader::Load()
	{
		//Create all the paths
		std::string vertexPath = GetPath().GetString() + ".vertex.glsl";
		std::string pixelPath = GetPath().GetString() + ".pixel.glsl";
		std::string tessellationControlPath = GetPath().GetString() + ".tesscontrol.glsl";

		//Read all shader files that can be found
		std::string m_VertexSource = "";
		if (std::filesystem::exists(vertexPath))
			m_VertexSource = ReadFile(vertexPath);

		std::string m_PixelSource = "";
		if (std::filesystem::exists(pixelPath))
			m_PixelSource = ReadFile(pixelPath);

		std::string m_TessellationControlSource = "";
		if (std::filesystem::exists(tessellationControlPath))
			m_TessellationControlSource = ReadFile(tessellationControlPath);

		//Check if the vertex and pixel files could be read. 
		//If not, we don't compile the shader.
		if (!m_VertexSource.empty() && !m_PixelSource.empty())
		{
			m_VertexID = CompileShader(m_VertexSource, GL_VERTEX_SHADER);
			m_PixelID = CompileShader(m_PixelSource, GL_FRAGMENT_SHADER);

			if(!m_TessellationControlSource.empty())
				m_TessellationControlID = CompileShader(m_TessellationControlSource, GL_TESS_CONTROL_SHADER);

			m_ProgramID = MakeProgram();
			CleanUpCompiledShaders();
		}
		else
		{
			Utility::Logger::Error("Shaders in directory {0} could not be found.", GetPath().GetString());
			return false;
		}



		return true;
	}

	bool Shader::Unload()
	{
		return false;
	}

	std::string Shader::ReadFile(const std::string& filePath)
	{
		std::stringstream ss = {};
		std::ifstream vertexStream(filePath);

		if (vertexStream.is_open())
			ss << vertexStream.rdbuf();
		else Utility::Logger::Error("Failed to open ifstream for file {0}.", filePath);
		return ss.str();
	}

	unsigned int Shader::CompileShader(const std::string& shaderSource, unsigned int shaderType)
	{

		unsigned int shaderID = glCreateShader(shaderType);
		const char* shaderSourceChars = shaderSource.c_str();
		LAG_GRAPHICS_EXCEPTION(glShaderSource(shaderID, 1, &shaderSourceChars, NULL));
		LAG_GRAPHICS_EXCEPTION(glCompileShader(shaderID));

		int compileStatus = 0;
		LAG_GRAPHICS_EXCEPTION(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus));
		if (compileStatus != GL_TRUE)
		{
			char shaderInfoLog[512];
			LAG_GRAPHICS_EXCEPTION(glGetShaderInfoLog(shaderID, 512, NULL, shaderInfoLog));
			Utility::Logger::Error("Failed to compile shader for {0} shaders: {1}", GetPath().GetString(), shaderInfoLog);
#ifdef DEBUG
			Utility::Logger::Info("Dumping shader source: \n{0}", shaderSource);
#endif
			return 0;
		}

		return shaderID;
	}

	unsigned int Shader::MakeProgram()
	{
		unsigned int programID = glCreateProgram();

		//The vertex and pixel shaders are always used. Because of that, we don't check the ID. 
		LAG_GRAPHICS_EXCEPTION(glAttachShader(programID, m_VertexID));
		LAG_GRAPHICS_EXCEPTION(glAttachShader(programID, m_PixelID));
		if(m_TessellationControlID > 0)
			LAG_GRAPHICS_EXCEPTION(glAttachShader(programID, m_TessellationControlID));


		LAG_GRAPHICS_EXCEPTION(glLinkProgram(programID));

		GLint programStatus;
		LAG_GRAPHICS_EXCEPTION(glGetProgramiv(programID, GL_LINK_STATUS, &programStatus));
		if (programStatus != GL_TRUE)
		{
			char programInfoLog[512];
			LAG_GRAPHICS_EXCEPTION(glGetProgramInfoLog(programID, 512, NULL, programInfoLog));
			Utility::Logger::Error("Failed to compile shader program for {0} shaders: {1}", GetPath().GetString(), programInfoLog);
			return 0;
		}

		else return programID;
	}

	void Shader::CleanUpCompiledShaders()
	{
		if (m_VertexID != 0)
			LAG_GRAPHICS_EXCEPTION(glDeleteShader(m_VertexID));
		if (m_PixelID != 0)
			LAG_GRAPHICS_EXCEPTION(glDeleteShader(m_PixelID));
		if (m_TessellationControlID != 0)
			LAG_GRAPHICS_EXCEPTION(glDeleteShader(m_TessellationControlID));
	}

	void Shader::Bind()
	{
		LAG_GRAPHICS_EXCEPTION(glUseProgram(m_ProgramID));
	}

	void Shader::Unbind()
	{
		LAG_GRAPHICS_EXCEPTION(glUseProgram(0));
	}

	void Shader::SetBool(const std::string& location, bool value)
	{
		LAG_GRAPHICS_EXCEPTION(glUniform1i(glGetUniformLocation(m_ProgramID, location.c_str()), value));
	}

	void Shader::SetInt(const std::string& location, int value)
	{
		LAG_GRAPHICS_EXCEPTION(glUniform1i(glGetUniformLocation(m_ProgramID, location.c_str()), value));
	}

	void Shader::SetFloat(const std::string& location, float value)
	{
		LAG_GRAPHICS_EXCEPTION(glUniform1f(glGetUniformLocation(m_ProgramID, location.c_str()), value));
	}

	void Shader::SetVec3(const std::string& location, glm::vec3 value)
	{
		LAG_GRAPHICS_EXCEPTION(glUniform3f(glGetUniformLocation(m_ProgramID, location.c_str()), value.x, value.y, value.z));
	}

	void Shader::SetMat4(const std::string& location, const glm::mat4& value)
	{
		LAG_GRAPHICS_EXCEPTION(glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, location.c_str()), 1, GL_FALSE, glm::value_ptr(value[0])));
	}

}