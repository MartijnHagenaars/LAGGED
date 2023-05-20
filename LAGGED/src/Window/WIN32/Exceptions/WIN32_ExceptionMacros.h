#pragma once
#include "Window/WIN32/Exceptions/WIN32_Exception.h"

#ifdef DEBUG
	#define LAG_WINDOW_EXCEPTION(hResult) if(FAILED(hResult)) throw WindowException(__LINE__, __FILE__, hResult);
	#define LAG_WINDOW_EXCEPTION_PREV() HRESULT hr = GetLastError(); if (FAILED(hr)) throw WindowException(HRESULT(hr), __LINE__, __FILE__);
#else
	#define LAG_WINDOW_EXCEPTION(hResult)
	#define LAG_WINDOW_EXCEPTION_PREV()
#endif
