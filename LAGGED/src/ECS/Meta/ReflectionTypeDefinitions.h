#pragma once
#include "entt/entt.hpp"
#include "ReflectionDefines.h"
#include "ReflectionTypeWidgets.h"

namespace LAG
{
	namespace Reflection
	{
		template <typename T>
		static void TypeAdapter(const char* name, entt::meta_any& value, const entt::meta_data& meta)
		{
			SetWidgetType<T>(meta, name, value.cast<T&>());
		}

		template <typename T>
		static entt::meta_factory<T> SetupReflectionType()
		{
			auto type = entt::meta<T>();
			type.template func<&Reflection::TypeAdapter<T>>(entt::hashed_string("EDITOR_WIDGET"));
			return type;
		}

		int InitializeReflectionTypes();
		static inline bool m_ReflectionTypeState = InitializeReflectionTypes();
	}


}