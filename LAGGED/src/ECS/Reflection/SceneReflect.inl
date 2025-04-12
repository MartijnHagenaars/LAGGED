#pragma once
#include <algorithm>

#include "Core/Defines.h"

namespace LAG
{
	template<typename Comp>
	inline ComponentReflectionSetup SceneReflect::RegisterComponent()
	{
		static_assert(!std::is_fundamental<Comp>::value, "Component type cannot be a fundamental type.");
		static_assert(std::is_default_constructible<Comp>::value, "Component type is not constructible.");

		ComponentID compID = Scene::GetComponentID<Comp>();
		auto it = m_ComponentProperties.find(compID);
		if (it != m_ComponentProperties.end())
			return ComponentReflectionSetup(it->second);

		auto compPropIt = m_ComponentProperties.emplace(compID, ComponentClassProperties({})).first;
		SceneReflect::ComponentClassProperties& props = compPropIt->second;
		return ComponentReflectionSetup(props);
	}

	template<typename Comp, typename Var>
	inline VariableReflectionSetup LAG::SceneReflect::RegisterVariable(Var Comp::* var)
	{
		static_assert(std::is_member_object_pointer<decltype(var)>::value, "Type needs to be a non-static member object pointer.");

		ComponentID compID = Scene::GetComponentID<Comp>();
		auto compIt = m_ComponentProperties.find(compID);
		if (compIt == m_ComponentProperties.end())
		{
			LAG_ASSERT("Cannot register variable: variable already registered.");
		}

		auto& varVec = compIt->second.varProperties;
		size_t byteOffset = reinterpret_cast<size_t>(&(reinterpret_cast<Comp*>(0)->*var));
		auto varIt = std::find_if(varVec.begin(), varVec.end(), [&byteOffset](ComponentVariableProperties& var) { return var.byteOffset == byteOffset; });

		if (varIt != varVec.end())
		{
			WARNING("Attempted to register variable {} again.", typeid(var).name());
			return VariableReflectionSetup(*varIt);
		}

		varVec.emplace_back(SceneReflect::ComponentVariableProperties());
		SceneReflect::ComponentVariableProperties& varProps = varVec[varVec.size() - 1];

		varProps.byteOffset = byteOffset;
		return VariableReflectionSetup(varProps);
	}
}
