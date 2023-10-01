#include "Precomp.h"
#include "GL_Shader.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "GL/glew.h"
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/Renderer/Exceptions/GL_GraphicsExceptionMacros.h"

LAG::Shader::Shader(std::string shaderPath)
{
	std::string vertexPath = shaderPath + ".vertex.glsl";
	std::string pixelPath = shaderPath + ".pixel.glsl";
	if (std::filesystem::exists(vertexPath) && std::filesystem::exists(pixelPath))
	{
		m_VertexSource = ReadFile(vertexPath);
		m_PixelSource = ReadFile(pixelPath);

		m_VertexID = CompileShader(m_VertexSource, GL_VERTEX_SHADER);
		m_PixelID = CompileShader(m_PixelSource, GL_VERTEX_SHADER);

		m_ProgramID = MakeProgram();
		CleanUpCompiledShaders();
	}
	else 
		Utility::Logger::Error("Shader(s) with name {0} could not be found.", shaderPath);
}

LAG::Shader::Shader(const ShaderData::GLShaderBase& shader)
{
	m_VertexID = CompileShader(shader.GetVertex(), GL_VERTEX_SHADER);
	m_PixelID = CompileShader(shader.GetPixel(), GL_FRAGMENT_SHADER);

	m_ProgramID = MakeProgram();
	CleanUpCompiledShaders();
}

std::string LAG::Shader::ReadFile(const std::string& filePath)
{
	std::stringstream ss = {};
	std::ifstream vertexStream(filePath);

	if (vertexStream.is_open())
		ss << vertexStream.rdbuf();
	else Utility::Logger::Error("Failed to open ifstream for file {0}.", filePath);
	return ss.str();
}

unsigned int LAG::Shader::CompileShader(const std::string& shaderSource, unsigned int shaderType)
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
		Utility::Logger::Error("Failed to compile shader: {0}", shaderInfoLog);
#ifdef DEBUG
		Utility::Logger::Info("Dumping shader source: \n{0}", shaderSource);
#endif
		return 0; 
	}

	return shaderID;
}

unsigned int LAG::Shader::MakeProgram()
{
	unsigned int programID = glCreateProgram();
	LAG_GRAPHICS_EXCEPTION(glAttachShader(programID, m_VertexID));
	LAG_GRAPHICS_EXCEPTION(glAttachShader(programID, m_PixelID));
	LAG_GRAPHICS_EXCEPTION(glLinkProgram(programID));

	GLint programStatus; 
	LAG_GRAPHICS_EXCEPTION(glGetProgramiv(programID, GL_LINK_STATUS, &programStatus));
	if (programStatus != GL_TRUE)
	{
		char programInfoLog[512];
		LAG_GRAPHICS_EXCEPTION(glGetProgramInfoLog(programID, 512, NULL, programInfoLog));
		Utility::Logger::Error("Failed to compile shader program: {0}", programInfoLog);
		return 0;
	}

	else return programID;
}

void LAG::Shader::CleanUpCompiledShaders()
{
	if (m_VertexID != 0)
		LAG_GRAPHICS_EXCEPTION(glDeleteShader(m_VertexID));
	if (m_PixelID != 0)
		LAG_GRAPHICS_EXCEPTION(glDeleteShader(m_PixelID));
}

void LAG::Shader::Bind()
{
	LAG_GRAPHICS_EXCEPTION(glUseProgram(m_ProgramID));
}

void LAG::Shader::Unbind()
{
	LAG_GRAPHICS_EXCEPTION(glUseProgram(0));
}

void LAG::Shader::SetBool(const std::string& location, bool value)
{
	LAG_GRAPHICS_EXCEPTION(glUniform1i(glGetUniformLocation(m_ProgramID, location.c_str()), value));
}

void LAG::Shader::SetInt(const std::string& location, int value)
{
	LAG_GRAPHICS_EXCEPTION(glUniform1i(glGetUniformLocation(m_ProgramID, location.c_str()), value));
}

void LAG::Shader::SetFloat(const std::string& location, float value)
{
	LAG_GRAPHICS_EXCEPTION(glUniform1f(glGetUniformLocation(m_ProgramID, location.c_str()), value));
}

void LAG::Shader::SetVec3(const std::string& location, glm::vec3 value)
{
	LAG_GRAPHICS_EXCEPTION(glUniform3f(glGetUniformLocation(m_ProgramID, location.c_str()), value.x, value.y, value.z));
}

void LAG::Shader::SetMat4(const std::string& location, const glm::mat4& value)
{
	LAG_GRAPHICS_EXCEPTION(glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, location.c_str()), 1, GL_FALSE, glm::value_ptr(value[0])));
}
