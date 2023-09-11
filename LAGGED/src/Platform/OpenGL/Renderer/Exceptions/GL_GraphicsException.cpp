#include "Precomp.h"
#include "GL_GraphicsException.h"
#include "GL/glew.h"

namespace LAG
{
	GraphicsException::GraphicsException(int exLine, const char* exFile, GLenum result) :
		ExceptionBase(exLine, exFile), m_Result(result)
	{
		m_ExceptionType = "Graphics (OpenGL) Exception";
	}

	GraphicsException::~GraphicsException()
	{}

	std::string GraphicsException::GetExceptionMessage() const
	{
		std::ostringstream ss;

		ss << "Exception of type \"" << m_ExceptionType << "\" was thrown!\n";
		ss << "\n[File]: " << GetExceptionFile();
		ss << "\n[Line]: " << GetExceptionLine();
		ss << "\n";
		ss << "\n[Error code]: " << std::to_string(m_Result);
		ss << "\n[Error description]: " << TranslateErrorMsg(m_Result);

		return ss.str();
	}

	std::string GraphicsException::TranslateErrorMsg(GLenum result)
	{
		switch (result)
		{
		case GL_NO_ERROR: return "No error";
		case GL_INVALID_ENUM: return "Invalid enum";
		case GL_INVALID_VALUE: return "Invalid value";
		case GL_INVALID_OPERATION: return "Invalid operation";
		case GL_STACK_OVERFLOW: return "Stack overflow";
		case GL_STACK_UNDERFLOW: return "Stack underflow";
		case GL_OUT_OF_MEMORY: return "Out of memory";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid framebuffer operation";
		default: return "Unknown error message.";
		}
	}
}