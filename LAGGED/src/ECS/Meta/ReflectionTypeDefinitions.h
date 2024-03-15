#pragma once
#include "entt/entt.hpp"
#include "ReflectionDefines.h"
#include "ReflectionTypeWidgets.h"

#include "ECS/Entity.h"

namespace LAG
{
	namespace Reflection
	{
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
			type.func<&Reflection::TypeAdapter<T>>(entt::hashed_string("EDITOR_WIDGET_DRAW"));
			return type;
		}

		int InitializeReflectionTypes();
		static inline bool m_ReflectionTypeState = InitializeReflectionTypes();
	}


}