#pragma once
#include <algorithm>

#include "Core/Defines.h"

namespace LAG
{
	inline std::unordered_map<ComponentID, SceneReflect::ComponentData>& SceneReflect::GetComponentProperties()
	{
		static std::unordered_map<ComponentID, ComponentData> compProps;
		return compProps;
	}

	template<typename Comp>
	inline ComponentReflectionSetup SceneReflect::RegisterComponent()
	{
		static_assert(!std::is_fundamental<Comp>::value, "Component type cannot be a fundamental type.");
		static_assert(std::is_default_constructible<Comp>::value, "Component type is not constructible.");
		
		auto& compProperties = GetComponentProperties();
		ComponentID compID = Scene::GetComponentID<Comp>();

		auto it = compProperties.find(compID);
		if (it != compProperties.end())
			return ComponentReflectionSetup(it->second);

		auto compPropIt = compProperties.emplace(compID, ComponentData({})).first;
		SceneReflect::ComponentData& props = compPropIt->second;
		return ComponentReflectionSetup(props);
	}

	template<typename Comp, typename Var>
	inline VariableReflectionSetup LAG::SceneReflect::RegisterVariable(Var Comp::* var)
	{
		static_assert(std::is_member_object_pointer<decltype(var)>::value, "Type needs to be a non-static member object pointer.");

		auto& compProperties = GetComponentProperties();
		ComponentID compID = Scene::GetComponentID<Comp>();

		auto compIt = compProperties.find(compID);
		if (compIt == compProperties.end())
		{
			LAG_ASSERT("Cannot register variable: variable already registered.");
		}

		auto& varVec = compIt->second.vars;
		size_t byteOffset = reinterpret_cast<size_t>(&(reinterpret_cast<Comp*>(0)->*var));
		auto varIt = std::find_if(varVec.begin(), varVec.end(), [&byteOffset](VariableData& var) { return var.byteOffset == byteOffset; });

		if (varIt != varVec.end())
		{
			WARNING("Attempted to register variable {} again.", typeid(var).name());
			return VariableReflectionSetup(*varIt);
		}

		varVec.emplace_back(SceneReflect::VariableData());
		SceneReflect::VariableData& varProps = varVec[varVec.size() - 1];

		varProps.byteOffset = byteOffset;
		return VariableReflectionSetup(varProps);
	}
}
