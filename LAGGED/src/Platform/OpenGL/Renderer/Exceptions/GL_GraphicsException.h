#pragma once
#include "Platform/Base/ExceptionBase.h"

typedef unsigned int GLenum;

namespace LAG
{
	class GraphicsException : public ExceptionBase
	{
	public:
		GraphicsException(int exLine, const char* exFile, GLenum result);
		~GraphicsException();

		std::string GetExceptionMessage() const override;
		std::string GetExceptionInfo() const;

	private:
		static std::string TranslateErrorMsg(GLenum result);

		uint32_t m_Result;
		std::string m_ExceptionInfo = "";
	};
}

