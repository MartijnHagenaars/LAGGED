#pragma once
#include "entt/entt.hpp"
#include "ReflectionComponentSetup.h"
#include "ReflectionTypeWidgets.h"

#include "ECS/Entity.h"

namespace LAG
{
	namespace Reflection
	{
		constexpr entt::hashed_string HANDLE_WIDGET_TYPE_FUNC = entt::hashed_string("HANDLE_WIDGET_TYPE");

		enum class WidgetModes
		{
			DRAW = 0,
			LOAD,
			UNLOAD
		};

		template <typename T>
		static void TypeAdapter(Entity& entity, entt::meta_any& value, const char* name, const Reflection::WidgetModes& mode)
		{
			switch (mode)
			{
			case Reflection::WidgetModes::DRAW:
				DrawWidgetType<T>(&entity, value.cast<T&>(), name);
				return;
			case Reflection::WidgetModes::LOAD:
				LoadWidgetType<T>(&entity, value.cast<T&>(), name);
				return;
			case Reflection::WidgetModes::UNLOAD:
				UnloadWidgetType<T>(&entity, value.cast<T&>(), name);
				return;
			}
		}

		template <typename T>
		static entt::meta_factory<T> SetupReflectionType()
		{
			auto type = entt::meta<T>();
			type.func<&Reflection::TypeAdapter<T>>(Reflection::HANDLE_WIDGET_TYPE_FUNC);
			return type;
		}

		int InitializeReflectionTypes();
		static inline bool m_ReflectionTypeState = InitializeReflectionTypes();
	}


}