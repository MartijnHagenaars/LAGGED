#include "ExceptionBase.h"
#include <sstream>

namespace LAG
{
	ExceptionBase::ExceptionBase(unsigned int exceptionLine, const char* exceptionFile) :
		m_ExceptionLine(exceptionLine), m_ExceptionFile(exceptionFile)
	{

	}

	char const* ExceptionBase::what() const
	{
		m_ExceptionBuffer = GetExceptionMessage();
		return m_ExceptionBuffer.c_str();
	}

	std::string ExceptionBase::GetExceptionMessage() const
	{
		std::ostringstream ss;

		ss << "Exception of type \"" << m_ExceptionType << "\" has been called!\n";
		ss << "\n[File]: " << GetExceptionFile();
		ss << "\n[Line]: " << GetExceptionLine();
		ss << "\n";
		ss << "\n[Description]: No description available.\n";

		return ss.str();
	}

	std::string ExceptionBase::GetExceptionFile() const
	{
		return m_ExceptionFile;
	}

	unsigned int ExceptionBase::GetExceptionLine() const
	{
		return m_ExceptionLine;
	}

	std::string ExceptionBase::GetExceptionType() const
	{
		return m_ExceptionType;
	}
}