#pragma once
#include <exception>
#include <string>
#include <assert.h>

namespace LAG
{
	class ExceptionBase : public std::exception
	{
	public:
		ExceptionBase(unsigned int exceptionLine, const char* exceptionFile);
		~ExceptionBase() {}

		//Override the what() function from std::exception to return a string containing exception info
		char const* what() const override;

		//Constructs and returns a string containing information about the exception
		virtual std::string GetExceptionMessage() const;

		//Get the file where the exception occurred. 
		std::string GetExceptionFile() const;

		//Get the line where the exception occurred. 
		unsigned int GetExceptionLine() const;

		//Get the type of exception.
		std::string GetExceptionType() const;

	protected:
		//Stores the type of exception. Needs to be changed depending on the exception class.
		std::string m_ExceptionType = "Undefined exception";

	private:
		unsigned int m_ExceptionLine = 0;
		std::string m_ExceptionFile = 0;

		//Exception buffer contains the "what". Basically the description of what the issue was. 
		//Needs to be mutable because otherwise it won't be editable in the what() function.
		mutable std::string m_ExceptionBuffer;
	};
}

