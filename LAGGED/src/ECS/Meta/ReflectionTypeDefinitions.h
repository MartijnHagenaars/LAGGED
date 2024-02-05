#pragma once
#include "entt/entt.hpp"
#include "ReflectionDefines.h"
#include "ReflectionTypeWidgets.h"

namespace LAG
{
	namespace Reflection
	{
		template <typename T>
		static void TypeAdapter(const char* name, entt::meta_any& value, const entt::meta_data& meta, Reflection::WidgetModes mode)
		{
			switch (mode)
			{
			case Reflection::WidgetModes::DRAW:
				DrawWidgetType<T>(meta, name, value.cast<T&>());
				return;
			case Reflection::WidgetModes::LOAD:
				LoadWidgetType<T>(meta, name, value.cast<T&>());
				return;
			case Reflection::WidgetModes::UNLOAD:
				UnloadWidgetType<T>(meta, name, value.cast<T&>());
				return;
			}
		}

		template <typename T>
		static entt::meta_factory<T> SetupReflectionType()
		{
			auto type = entt::meta<T>();
			type.func<&Reflection::TypeAdapter<T>>(entt::hashed_string("EDITOR_WIDGET_DRAW"));
			return type;
		}

		int InitializeReflectionTypes();
		static inline bool m_ReflectionTypeState = InitializeReflectionTypes();
	}


}