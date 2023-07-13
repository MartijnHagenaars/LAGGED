#include "Precomp.h"
#include "DX12_WindowException.h"
#include <sstream>

namespace LAG
{
	WindowException::WindowException(HRESULT hResult, unsigned int exceptionLine, const char* exceptionFile) :
		ExceptionBase(exceptionLine, exceptionFile), m_HResult(hResult)
	{
		m_ExceptionType = "Window (Win32) Exception";
	}

	std::string WindowException::GetExceptionMessage() const
	{
		std::ostringstream ss;

		ss << "Exception of type \"" << m_ExceptionType << "\" was thrown!\n";
		ss << "\n[File]: " << GetExceptionFile();
		ss << "\n[Line]: " << GetExceptionLine();
		ss << "\n";
		ss << "\n[Error code]: " << std::to_string(m_HResult);
		ss << "\n[Error description]: " << TranslateWinAPIErrors(m_HResult);

		return ss.str();
	}

	std::string WindowException::TranslateWinAPIErrors(HRESULT hResult) const
	{
		if (hResult != 0)
		{
			LPSTR errorMsgBuffer = nullptr;

			//Allocate some memory and retrieve the error message from it. 
			size_t msgSize = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errorMsgBuffer, 0, NULL);

			if (msgSize <= 0) return std::string("No information available.");

			//Copy the error message into a std::string.
			std::string erorrMsg(errorMsgBuffer, msgSize);

			//Free the Win32's string's buffer.
			LocalFree(errorMsgBuffer);

			return erorrMsg;
		}
		return std::string("Error code 0. No information available.");
	}
}