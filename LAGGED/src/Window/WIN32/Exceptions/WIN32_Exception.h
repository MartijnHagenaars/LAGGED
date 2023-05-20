#pragma once
#include "Platform/ExceptionBase.h"
#include "Platform/PlatformIncludes.h"

namespace LAG
{
	class WindowException : public ExceptionBase
	{
	public:
		WindowException(HRESULT hResult, unsigned int exceptionLine, const char* exceptionFile);
		~WindowException() {}

		std::string GetExceptionMessage() const override;

	private:
		std::string TranslateWinAPIErrors(HRESULT hResult) const;

		HRESULT m_HResult;
	};
}