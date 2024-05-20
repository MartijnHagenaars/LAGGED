#pragma once
#include "Utility/Logger.h"

//Renderer defines
#define TOTAL_POINT_LIGHTS 3

//Assertion defines
#ifdef DEBUG
#define LAG_ASSERT(AssertMessage) { LAG::Logger::Critical("Assertion called on line {0} in file {1}: {2}", __LINE__, __FILE__, AssertMessage); __debugbreak(); }
#else
#define LAG_ASSERT(AssertMessage)
#endif

#ifdef DEBUG
#define DEBUG_OPERATION(x) x
#else
#define DEBUG_OPERATION(x)
#endif