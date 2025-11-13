#pragma once
#include "Utility/Logger.h"

//Renderer defines
#define TOTAL_POINT_LIGHTS 3

using Hash32 = uint32_t;
using Hash64 = uint64_t;

//Assertion defines
#ifdef DEBUG
#define LAG_ASSERT(AssertMessage) { CRITICAL("Assertion called on line {0} in file {1}: {2}", __LINE__, __FILE__, AssertMessage); __debugbreak(); }
#else
#define LAG_ASSERT(AssertMessage) {}
#endif

#ifdef DEBUG
#define DEBUG_OPERATION(x) x
#else
#define DEBUG_OPERATION(x)
#endif

namespace LAG
{
	enum class ErrResult
	{
		SUCCESS = 0,
		ERR_INIT_FAIL,
		ERR_UNDEFINED
	};
}