#pragma once
#include <algorithm>

#include "Core/Defines.h"
#include "Utility/Hash.h"

namespace LAG
{
	template<typename Comp>
	inline ComponentReflectionSetup SceneReflect::ReflectComponent()
	{
		static_assert(!std::is_fundamental<Comp>::value, "Component type cannot be a fundamental type.");
		static_assert(std::is_default_constructible<Comp>::value, "Component type is not constructible.");
		
		// Ensure that the component type has been registered before we proceed
		constexpr TypeID typeID = Scene::GetTypeID<Comp>();
		if (const auto& typeInfoIt = Scene::s_TypeInfo.find(typeID); typeInfoIt == Scene::s_TypeInfo.end())
			Scene::RegisterType<Comp>();

		// Return early in case component is already reflected
		auto& reflCompInfoMap = Scene::s_ReflectedCompInfo;
		if (const auto& it = reflCompInfoMap.find(typeID); it != reflCompInfoMap.end())
		{
			CRITICAL("Attempted to reflect component '{}', but this operation has already been completed. Reflecting a component more than once is not allowed.", typeid(Comp).name());
			return ComponentReflectionSetup(it->second);
		}

		auto& reflCompInfo = reflCompInfoMap[typeID];
		reflCompInfo.props.displayName = typeid(Comp).name();
		return ComponentReflectionSetup(reflCompInfo);
	}

	template<typename Comp, typename Var>
	inline VariableReflectionSetup LAG::SceneReflect::ReflectVariable(Var Comp::* var)
	{
		static_assert(std::is_member_object_pointer<decltype(var)>::value, "Type needs to be a non-static member object pointer.");

		// Ensure that the component type has been registered before we proceed
		constexpr TypeID compTypeID = Scene::GetTypeID<Comp>();
		auto reflCompInfoIt = Scene::s_ReflectedCompInfo.find(compTypeID);
		if (reflCompInfoIt == Scene::s_ReflectedCompInfo.end())
		{
			CRITICAL("Component type \"{}\" is not registered. Make sure this is set-up correctly.", typeid(Comp).name());
			ReflectComponent<Comp>();
		}

		// Also ensure that the variable type has been registered before we proceed
		constexpr TypeID varTypeID = Scene::GetTypeID<Var>();
		if (const auto& typeInfoIt = Scene::s_TypeInfo.find(varTypeID); typeInfoIt == Scene::s_TypeInfo.end())
			Scene::RegisterType<Var>();

		// Check if this variable has already been reflected
		auto& memberVec = reflCompInfoIt->second.members;
		size_t byteOffset = reinterpret_cast<size_t>(&(reinterpret_cast<Comp*>(0)->*var));
		auto varIt = std::find_if(memberVec.begin(), memberVec.end(), [&byteOffset](ReflectedCompInfo::MemberInfo& var) { return var.byteOffset == byteOffset; });

		if (varIt != memberVec.end())
		{
			WARNING("Attempted to register variable {} again.", typeid(var).name());
			return VariableReflectionSetup(*varIt);
		}

		ReflectedCompInfo::MemberInfo& memberInfo = memberVec.emplace_back(ReflectedCompInfo::MemberInfo());
		memberInfo.props.displayName = typeid(Var).name();
		memberInfo.typeID = varTypeID;
		memberInfo.byteOffset = byteOffset;

		return VariableReflectionSetup(memberInfo);
	}
}
