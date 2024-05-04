#pragma once
#include "entt/entt.hpp"
#include "ReflectionTypeDefinitions.h"

namespace LAG
{
	namespace Reflection
	{
		constexpr entt::hashed_string DISPLAY_NAME = entt::hashed_string("DISPLAY_NAME");


#define LAG_REGISTER_COMPONENT(componentType)									\
		bool Setup##componentType()												\
	{																			\
		printf("Hello from %s function!\n", #componentType);					\
		return true;															\
	}																			\
		bool Setup##componentType##Success = Setup##componentType();			\

#define LAG_REGISTER_VARIABLE(variable)

	}
}