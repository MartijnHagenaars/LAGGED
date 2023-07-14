#pragma once
#include "DX12_Exception.h"

#ifdef DEBUG
	#define LAG_GRAPHICS_EXCEPTION(hResult) if(FAILED(hResult)) throw GraphicsException(__LINE__, __FILE__, HRESULT(hResult) );
	#define LAG_GRAPHICS_EXCEPTION_PREV() HRESULT hr = GetLastError(); if(FAILED(hr)) throw GraphicsException(__LINE__, __FILE__, HRESULT(GetLastError()));
	
	//#define LAG_GRAPHICS_EXCEPTION_TEST(funcCall) pDxgiInfoMan->setNewFrame(); (funcCall); { std::vector<std::string> strVec = pDxgiInfoMan->getQueueMessages(); if(!strVec.empty()) throw GraphicsInfoException(__LINE__, __FILE__, strVec); }
#else
	#define LAG_GRAPHICS_EXCEPTION(hResult) hResult
	#define LAG_GRAPHICS_EXCEPTION_PREV() 
	
	//#define LAG_GRAPHICS_EXCEPTION_TEST(funcCall)
#endif
