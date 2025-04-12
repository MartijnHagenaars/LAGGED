#pragma once
#include <type_traits>

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
			return ComponentReflectionSetup(&it->second);

		auto compPropIt = m_ComponentProperties.emplace(compID, ComponentClassProperties({})).first;
		SceneReflect::ComponentClassProperties* props = &compPropIt->second;
		return ComponentReflectionSetup(props);
	}

	template<typename Comp, typename Var>
	inline VariableReflectionSetup LAG::SceneReflect::RegisterVariable(Var Comp::* var)
	{
		static_assert(std::is_member_object_pointer<decltype(var)>::value, "Type needs to be a non-static member object pointer.");

		return VariableReflectionSetup(nullptr);
	}
}
