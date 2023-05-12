#pragma once

//Declspec defines
#ifdef BUILD_DLL
#define LAG_API __declspec(dllexport)
#else
#define LAG_API __declspec(dllimport)
#endif

//Assertion defines
#ifdef DEBUG
#define LAG_ASSERT(AssertMessage) std::cerr << "Assertion called on line " << __LINE__ << " in file " << __FILE__ << ": \n   > " << AssertMessage << std::endl; __debugbreak();
#else
#define LAG_ASSERT(AssertMessage)
#endif

#ifdef DEBUG
#define DEBUG_OPERATION(x) x
#else
#define DEBUG_OPERATION(x)
#endif