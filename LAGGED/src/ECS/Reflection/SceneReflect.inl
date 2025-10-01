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

		auto& compReflData = Scene::s_ReflectTypes;
		if (const auto& it = compReflData.find(compID); it != compReflData.end())
		{
			CRITICAL("Attempted to reflect component '{}', but this operation has already been completed. Reflecting a component more than once is not allowed.", typeid(Comp).name());
			return ComponentReflectionSetup(it->second);
		}

		auto& compRefl = compReflData[compID];
		compRefl.props.displayName = typeid(Comp).name();
		return ComponentReflectionSetup(compRefl);
	}

	template<typename Comp, typename Var>
	inline VariableReflectionSetup LAG::SceneReflect::ReflectVariable(Var Comp::* var)
	{
		static_assert(std::is_member_object_pointer<decltype(var)>::value, "Type needs to be a non-static member object pointer.");

		ComponentID compID = Scene::GetComponentID<Comp>();
		auto compReflData = Scene::s_ReflectTypes.find(compID);
		if (compReflData == Scene::s_ReflectTypes.end())
		{
			CRITICAL("Component type \"{}\" is not registered. Make sure this is set-up correctly.", typeid(Comp).name());
			ReflectComponent<Comp>();
		}

		Hash64 memberTypeHash = GetTypeHash64<Var>();
		auto memberReflData = Scene::s_ReflectMembers.find(memberTypeHash);
		if (memberReflData == Scene::s_ReflectMembers.end())
		{
			auto& reflectMemberData = Scene::s_ReflectMembers[memberTypeHash];
			reflectMemberData.VoidToAny = [](void* data) { return std::any(*static_cast<Var*>(data)); };
		}

		auto& varVec = compReflData->second.members;
		size_t byteOffset = reinterpret_cast<size_t>(&(reinterpret_cast<Comp*>(0)->*var));
		auto varIt = std::find_if(varVec.begin(), varVec.end(), [&byteOffset](ReflectionData::MemberData& var) { return var.byteOffset == byteOffset; });

		if (varIt != varVec.end())
		{
			WARNING("Attempted to register variable {} again.", typeid(var).name());
			return VariableReflectionSetup(*varIt);
		}

		varVec.emplace_back(ReflectionData::MemberData());
		ReflectionData::MemberData& varData = varVec[varVec.size() - 1];
		varData.props.displayName = typeid(Var).name();
		varData.typeID = memberTypeHash;
		varData.byteOffset = byteOffset;

		return VariableReflectionSetup(varData);
	}
}
