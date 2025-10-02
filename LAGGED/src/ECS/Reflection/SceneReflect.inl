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
		
		ComponentID compID = Scene::GetComponentID<Comp>();
		if (const auto& compDataIt = Scene::s_ComponentMap.find(compID); compDataIt == Scene::s_ComponentMap.end())
			Scene::RegisterComponent<Comp>();

		auto& reflCompInfoMap = Scene::s_ReflectedCompInfo;
		if (const auto& it = reflCompInfoMap.find(compID); it != reflCompInfoMap.end())
		{
			CRITICAL("Attempted to reflect component '{}', but this operation has already been completed. Reflecting a component more than once is not allowed.", typeid(Comp).name());
			return ComponentReflectionSetup(it->second);
		}

		auto& reflCompInfo = reflCompInfoMap[compID];
		reflCompInfo.props.displayName = typeid(Comp).name();
		return ComponentReflectionSetup(reflCompInfo);
	}

	template<typename Comp, typename Var>
	inline VariableReflectionSetup LAG::SceneReflect::ReflectVariable(Var Comp::* var)
	{
		static_assert(std::is_member_object_pointer<decltype(var)>::value, "Type needs to be a non-static member object pointer.");

		ComponentID compID = Scene::GetComponentID<Comp>();
		auto reflCompInfo = Scene::s_ReflectedCompInfo.find(compID);
		if (reflCompInfo == Scene::s_ReflectedCompInfo.end())
		{
			CRITICAL("Component type \"{}\" is not registered. Make sure this is set-up correctly.", typeid(Comp).name());
			ReflectComponent<Comp>();
		}

		Hash64 memberTypeHash = GetTypeHash64<Var>();
		auto reflTypeInfo = Scene::s_ReflectedTypeInfo.find(memberTypeHash);
		if (reflTypeInfo == Scene::s_ReflectedTypeInfo.end())
		{
			auto& newReflTypeInfo = Scene::s_ReflectedTypeInfo[memberTypeHash];
			newReflTypeInfo.VoidToAny = [](void* data) { return std::any(*static_cast<Var*>(data)); };
		}

		auto& memberVec = reflCompInfo->second.members;
		size_t byteOffset = reinterpret_cast<size_t>(&(reinterpret_cast<Comp*>(0)->*var));
		auto varIt = std::find_if(memberVec.begin(), memberVec.end(), [&byteOffset](ReflectedCompInfo::MemberInfo& var) { return var.byteOffset == byteOffset; });

		if (varIt != memberVec.end())
		{
			WARNING("Attempted to register variable {} again.", typeid(var).name());
			return VariableReflectionSetup(*varIt);
		}

		memberVec.emplace_back(ReflectedCompInfo::MemberInfo());
		ReflectedCompInfo::MemberInfo& memberInfo = memberVec[memberVec.size() - 1];
		memberInfo.props.displayName = typeid(Var).name();
		memberInfo.typeID = memberTypeHash;
		memberInfo.byteOffset = byteOffset;

		return VariableReflectionSetup(memberInfo);
	}
}
