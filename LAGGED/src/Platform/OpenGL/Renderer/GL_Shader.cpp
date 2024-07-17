#include "GL_Shader.h"

#include "FileIO/FileIO.h"
#include "Utility/Logger.h"

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "GL_ErrorChecks.h"

namespace LAG
{
	Shader::Shader(const HashedString& shaderPath) : ShaderBase(shaderPath)
	{
	}

	Shader::~Shader()
	{
	}

	bool Shader::Load()
	{
		//Read all shader files that can be found
		std::string fileName = GetPath().GetString().substr(GetPath().GetString().find_last_of('/') + 1, GetPath().GetString().length());
		std::string m_VertexSource = FileIO::Read(FileIO::Directory::Shaders, fileName + ".vertex.glsl");
		std::string m_PixelSource = FileIO::Read(FileIO::Directory::Shaders, fileName + ".pixel.glsl");

		//Check if the vertex and pixel files could be read. 
		//If not, we don't compile the shader.
		if (!m_VertexSource.empty() && !m_PixelSource.empty())
		{
			m_VertexID = CompileShader(m_VertexSource, GL_VERTEX_SHADER);
			m_PixelID = CompileShader(m_PixelSource, GL_FRAGMENT_SHADER);
			if (m_VertexID == 0 || m_PixelID == 0)
				return false;

			m_ProgramID = MakeProgram();
			CleanUpCompiledShaders();
			return true;
		}
		else
		{
			ERROR("Shaders in directory {0} could not be read/found.", GetPath().GetString());
			return false;
		}
	}

	bool Shader::Unload()
	{
		Unbind();
		LAG_GRAPHICS_CHECK(glDeleteProgram(m_ProgramID));

		return true;
	}

	bool Shader::Reload()
	{
		unsigned int prevProgramID = m_ProgramID;

		//Check if we can load the new shader files.
		if (!Load())
		{
			ERROR("Failed to reload shader: {0}", GetPath().GetString());
			CleanUpCompiledShaders();
			return false;
		}

		//Since the new shader has loaded correctly, the old one can be removed.
		Unbind();
		LAG_GRAPHICS_CHECK(glDeleteProgram(prevProgramID));

		return true;
	}

	unsigned int Shader::CompileShader(const std::string& shaderSource, unsigned int shaderType)
	{
		unsigned int shaderID = glCreateShader(shaderType);
		const char* shaderSourceChars = shaderSource.c_str();
		LAG_GRAPHICS_CHECK(glShaderSource(shaderID, 1, &shaderSourceChars, NULL));
		LAG_GRAPHICS_CHECK(glCompileShader(shaderID));

		int compileStatus = 0;
		LAG_GRAPHICS_CHECK(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus));
		if (compileStatus != GL_TRUE)
		{
			//Print an error message. After that, delete the shader. 
			//Also dump the entire shader code to the console if we're running debug. Might be useful to see the entire shader when debugging.
			char shaderInfoLog[512];
			LAG_GRAPHICS_CHECK(glGetShaderInfoLog(shaderID, 512, NULL, shaderInfoLog));
			ERROR("Failed to compile shader for {0} shaders: {1}", GetPath().GetString(), shaderInfoLog);
#ifdef DEBUG
			INFO("Dumping shader source: \n{0}", shaderSource);
#endif
			glDeleteShader(shaderID);
			return 0;
		}

		return shaderID;
	}

	unsigned int Shader::MakeProgram()
	{
		unsigned int programID = glCreateProgram();

		//Attach the vertex and pixel shader. After that, link the newly created program.
		LAG_GRAPHICS_CHECK(glAttachShader(programID, m_VertexID));
		LAG_GRAPHICS_CHECK(glAttachShader(programID, m_PixelID));

		LAG_GRAPHICS_CHECK(glLinkProgram(programID));

		//Check if everything executed correctly.
		GLint programStatus;
		LAG_GRAPHICS_CHECK(glGetProgramiv(programID, GL_LINK_STATUS, &programStatus));
		if (programStatus != GL_TRUE)
		{
			//Print an error message. After that, delete the shader program.
			char programInfoLog[512];
			LAG_GRAPHICS_CHECK(glGetProgramInfoLog(programID, 512, NULL, programInfoLog));
			CRITICAL("Failed to compile shader program for {0} shaders: {1}", GetPath().GetString(), programInfoLog);

			glDeleteProgram(programID);
			return 0;
		}

		else return programID;
	}

	void Shader::CleanUpCompiledShaders()
	{
		//Since there might be some left-over memory after trying to (re)compile shaders, we want to ensure that everything is deleted properly.

		if (m_VertexID != 0)
			LAG_GRAPHICS_CHECK(glDeleteShader(m_VertexID));
		m_VertexID = 0;

		if (m_PixelID != 0)
			LAG_GRAPHICS_CHECK(glDeleteShader(m_PixelID));
		m_PixelID = 0;
	}

	void Shader::Bind()
	{
		LAG_GRAPHICS_CHECK(glUseProgram(m_ProgramID));
	}

	void Shader::Unbind()
	{
		LAG_GRAPHICS_CHECK(glUseProgram(0));
	}

	void Shader::SetBool(const std::string& location, bool value)
	{
		LAG_GRAPHICS_CHECK(glUniform1i(glGetUniformLocation(m_ProgramID, location.c_str()), value));
	}

	void Shader::SetInt(const std::string& location, int value)
	{
		LAG_GRAPHICS_CHECK(glUniform1i(glGetUniformLocation(m_ProgramID, location.c_str()), value));
	}

	void Shader::SetFloat(const std::string& location, float value)
	{
		LAG_GRAPHICS_CHECK(glUniform1f(glGetUniformLocation(m_ProgramID, location.c_str()), value));
	}

	void Shader::SetVec3(const std::string& location, glm::vec3 value)
	{
		LAG_GRAPHICS_CHECK(glUniform3f(glGetUniformLocation(m_ProgramID, location.c_str()), value.x, value.y, value.z));
	}

	void Shader::SetMat4(const std::string& location, const glm::mat4& value)
	{
		LAG_GRAPHICS_CHECK(glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, location.c_str()), 1, GL_FALSE, glm::value_ptr(value[0])));
	}

}