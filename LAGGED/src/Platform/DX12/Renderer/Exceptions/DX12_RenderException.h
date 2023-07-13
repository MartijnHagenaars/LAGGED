#pragma once
#include "Platform/Base/ExceptionBase.h"
#include <vector>

namespace LAG
{
	class GraphicsException : public ExceptionBase
	{
	public:
		GraphicsException(int exLine, const char* exFile, HRESULT hResult);
		~GraphicsException();

		std::string GetExceptionMessage() const override;
		std::string GetExceptionInfo() const;

	private:
		static std::string TranslateErrorMsg(HRESULT hResult);

		HRESULT m_HResult;
		std::string m_ExceptionInfo = "";
	};
}