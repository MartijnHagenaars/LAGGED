#pragma once
#include "Core/Engine.h"

namespace LAG
{
	constexpr Hash64 ADD_COMPONENT = StringToHash64("ADD_COMPONENT");
	constexpr Hash64 REMOVE_COMPONENT = StringToHash64("REMOVE_COMPONENT");

	namespace ReflectInternal
	{
		// TODO: ENSURE THAT THIS OPERATION CAN ONLY BE PERFORMED ON A COMPONENT AND NOT A REGULAR TYPE LIKE INT, FLOAT, ETC.
		template <typename T>
		[[maybe_unused]] static void AddComponent(EntityID entityID)
		{
			GetScene()->AddComponent<T>(entityID);
		}

		// TODO: ENSURE THAT THIS OPERATION CAN ONLY BE PERFORMED ON A COMPONENT AND NOT A REGULAR TYPE LIKE INT, FLOAT, ETC.
		template <typename T>
		[[maybe_unused]] static void RemoveComponent(EntityID entityID)
		{
			GetScene()->RemoveComponent<T>(entityID);
		}

		//// TODO: ADD DUPLICATE FUNCTION

		template <typename T>
		static void SetupCommonOperations()
		{
			//TODO: This needs to only be a func on components
			SceneReflect::RegisterFunc<T, &ReflectInternal::AddComponent<T>>(ADD_COMPONENT);
			SceneReflect::RegisterFunc<T, &ReflectInternal::RemoveComponent<T>>(REMOVE_COMPONENT);
		}
	}
}