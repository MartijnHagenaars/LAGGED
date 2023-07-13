#pragma once
#include "DX12_WindowException.h"

#ifdef DEBUG
	#define LAG_WINDOW_EXCEPTION(hResult) if(FAILED(hResult)) throw WindowException(__LINE__, __FILE__, hResult);
	#define LAG_WINDOW_EXCEPTION_PREV() HRESULT hr = GetLastError(); if (FAILED(hr)) throw WindowException(HRESULT(hr), __LINE__, __FILE__);
#else
	#define LAG_WINDOW_EXCEPTION(hResult)
	#define LAG_WINDOW_EXCEPTION_PREV()
#endif
