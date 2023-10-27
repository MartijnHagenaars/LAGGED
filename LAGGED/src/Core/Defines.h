#pragma once

//Declspec defines
#ifdef BUILD_DLL
#define LAG_API  
#else
#define LAG_API  
#endif

//Assertion defines
#ifdef DEBUG
#define LAG_ASSERT(AssertMessage) { LAG::Utility::Logger::Critical("Assertion called on line {0} in file {1}: {2}", __LINE__, __FILE__, AssertMessage); __debugbreak(); }
#else
#define LAG_ASSERT(AssertMessage)
#endif

#ifdef DEBUG
#define DEBUG_OPERATION(x) x
#else
#define DEBUG_OPERATION(x)
#endif