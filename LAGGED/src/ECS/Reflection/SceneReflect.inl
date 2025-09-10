#pragma once
#include <algorithm>

#include "Core/Defines.h"

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

		auto& compProperties = SceneReflect::Get().GetComponentProperties();
		if (const auto& it = compProperties.find(compID); it != compProperties.end())
		{
			CRITICAL("Attempted to reflect component '{}', but this operation has already been completed. Reflecting a component more than once is not allowed.", typeid(Comp).name());
			return ComponentReflectionSetup(it->second);
		}

		auto compPropIt = compProperties.emplace(compID, ComponentReflectionData({})).first;
		SceneReflect::ComponentReflectionData& compData = compPropIt->second;
		compData.props.displayName = typeid(Comp).name();

		return ComponentReflectionSetup(compData);
	}

	template<typename Comp, typename Var>
	inline VariableReflectionSetup LAG::SceneReflect::ReflectVariable(Var Comp::* var)
	{
		static_assert(std::is_member_object_pointer<decltype(var)>::value, "Type needs to be a non-static member object pointer.");

		ComponentID compID = Scene::GetComponentID<Comp>();
		auto& compProperties = SceneReflect::Get().GetComponentProperties();

		auto compIt = compProperties.find(compID);
		if (compIt == compProperties.end())
		{
			LAG_ASSERT("Cannot register variable: variable already registered.");
		}

		auto& varVec = compIt->second.vars;
		size_t byteOffset = reinterpret_cast<size_t>(&(reinterpret_cast<Comp*>(0)->*var));
		auto varIt = std::find_if(varVec.begin(), varVec.end(), [&byteOffset](VariableReflectionData& var) { return var.byteOffset == byteOffset; });

		if (varIt != varVec.end())
		{
			WARNING("Attempted to register variable {} again.", typeid(var).name());
			return VariableReflectionSetup(*varIt);
		}

		varVec.emplace_back(SceneReflect::VariableReflectionData());
		SceneReflect::VariableReflectionData& varData = varVec[varVec.size() - 1];
		varData.props.displayName = typeid(Var).name();
		varData.byteOffset = byteOffset;

		return VariableReflectionSetup(varData);
	}
}
