#pragma once
#include "Utility/Logger.h"

//Renderer defines
#define TOTAL_POINT_LIGHTS 3

using Hash32 = uint32_t;
using Hash64 = uint64_t;


/////////////////////////////////
// Enum generation definitions //
/////////////////////////////////

#define DEFINE_ENUM_AND_STRINGS(NAME, ...)          \
    enum class NAME : short { __VA_ARGS__ };        \
    static constexpr auto s_##NAME##Strings = MakeArrayFromArgs(FOR_EACH(STRING_ITEM, __VA_ARGS__))


//////////////////////////////////////////
// Enum generation internal definitions //
//////////////////////////////////////////

// TODO: Move this somewhere else. This is not a define! :( 
template <typename... Ts>
constexpr auto MakeArrayFromArgs(Ts&&... ts)
{
	return std::array<const char*, sizeof...(Ts)>{ std::forward<Ts>(ts)... };
}

// Used to force preprocessor to expand other macro expansons
#define INTERNAL_EXPAND(x) x

#define INTERNAL_FE_1(OPERATION, X) OPERATION(X)
#define INTERNAL_FE_2(OPERATION, X, ...) OPERATION(X), INTERNAL_EXPAND(INTERNAL_FE_1(OPERATION, __VA_ARGS__))
#define INTERNAL_FE_3(OPERATION, X, ...) OPERATION(X), INTERNAL_EXPAND(INTERNAL_FE_2(OPERATION, __VA_ARGS__))
#define INTERNAL_FE_4(OPERATION, X, ...) OPERATION(X), INTERNAL_EXPAND(INTERNAL_FE_3(OPERATION, __VA_ARGS__))
#define INTERNAL_FE_5(OPERATION, X, ...) OPERATION(X), INTERNAL_EXPAND(INTERNAL_FE_4(OPERATION, __VA_ARGS__))
#define INTERNAL_FE_6(OPERATION, X, ...) OPERATION(X), INTERNAL_EXPAND(INTERNAL_FE_5(OPERATION, __VA_ARGS__))
#define INTERNAL_FE_7(OPERATION, X, ...) OPERATION(X), INTERNAL_EXPAND(INTERNAL_FE_6(OPERATION, __VA_ARGS__))
#define INTERNAL_FE_8(OPERATION, X, ...) OPERATION(X), INTERNAL_EXPAND(INTERNAL_FE_7(OPERATION, __VA_ARGS__))

// Expandable as needed
#define INTERNAL_GET_FE_DEFINITION(_1,_2,_3,_4,_5,_6,_7,_8,NAME,...) NAME

#define FOR_EACH(OPERATION, ...) \
    INTERNAL_EXPAND(INTERNAL_GET_FE_DEFINITION(__VA_ARGS__, \
        INTERNAL_FE_8, INTERNAL_FE_7, INTERNAL_FE_6, INTERNAL_FE_5, INTERNAL_FE_4, INTERNAL_FE_3, INTERNAL_FE_2, INTERNAL_FE_1 \
    )(OPERATION, __VA_ARGS__))

#define STRING_ITEM(X) #X


///////////////////////////
// Debugging definitions //
///////////////////////////

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