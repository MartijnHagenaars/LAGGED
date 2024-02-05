#pragma once
#include "entt/entt.hpp"
#include "ReflectionTypeDefinitions.h"

namespace LAG
{
	namespace Reflection
	{
		enum class WidgetModes
		{
			DRAW = 0,
			LOAD,
			UNLOAD
		};

		constexpr entt::hashed_string DISPLAY_NAME = entt::hashed_string("DISPLAY_NAME");

		constexpr entt::hashed_string EDITOR_WIDGET_DRAW = entt::hashed_string("EDITOR_WIDGET_DRAW");
		constexpr entt::hashed_string EDITOR_WIDGET_LOAD = entt::hashed_string("EDITOR_WIDGET_LOAD");
		constexpr entt::hashed_string EDITOR_WIDGET_UNLOAD = entt::hashed_string("EDITOR_WIDGET_UNLOAD");
	}
}