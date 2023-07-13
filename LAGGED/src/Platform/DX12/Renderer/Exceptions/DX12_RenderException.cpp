#include "Precomp.h"
#include "DX12_RenderException.h"
#include "DX12_InfoQueueManager.h"

namespace LAG
{
	GraphicsException::GraphicsException(int exLine, const char* exFile, HRESULT hResult) : 
		ExceptionBase(exLine, exFile), m_HResult(hResult)
	{
		//Get the vec and the size
		std::vector<std::string> infoQueueVec = InfoQueueManager::GetExceptionInfoMessages();
		uint64_t infoQueueVecSize = infoQueueVec.size();

		if (infoQueueVecSize <= 0) {
			m_ExceptionInfo = "No extra info available.\n";
		}
		else {
			for (int i = 0; i < infoQueueVecSize; i++) {
				std::stringstream ss;
				ss << "[Issue " << i << "]: ";
				ss << infoQueueVec[i];
				ss << "\n";
				m_ExceptionInfo += ss.str();
			}
		}
	}

	GraphicsException::~GraphicsException()
	{

	}

	std::string GraphicsException::GetExceptionMessage() const
	{
		std::ostringstream ss;
		ss << "Graphics exception was thrown!" <<
			"\n[File]: " << GetExceptionFile() <<
			"\n[Line]: " << GetExceptionLine() <<
			"\n[Error code]: " << m_HResult <<
			"\n[Description]: " << TranslateErrorMsg(m_HResult) <<
			"\n[Extra info]: \n" << GetExceptionInfo();
		return ss.str();
	}

	std::string GraphicsException::GetExceptionInfo() const
	{
		return m_ExceptionInfo;
	}

	std::string GraphicsException::TranslateErrorMsg(HRESULT hResult)
	{
		if (hResult != 0)
		{
			LPSTR errorMsgBuffer = nullptr;

			//Ask Win32 to give us the string version of that message ID.
			//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
			size_t msgSize = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errorMsgBuffer, 0, NULL);

			//Copy the error message into a std::string.
			std::string message(errorMsgBuffer, msgSize);

			//Free the Win32's string's buffer.
			LocalFree(errorMsgBuffer);

			if (msgSize <= 0) return std::string("No information available.");
			else return message;
		}
		return std::string("Error code 0. No information available.");
	}
}