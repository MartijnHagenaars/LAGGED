#pragma once
#include "SceneReflect.h"

#include <algorithm>

namespace LAG
{
	template<typename Comp>
	inline ComponentReflectionSetup SceneReflect::ReflectComponent()
	{
		static_assert(!std::is_fundamental<Comp>::value, "Component type cannot be a fundamental type.");
		static_assert(std::is_default_constructible<Comp>::value, "Component type is not constructible.");
		
		TypeInfo* typeInfo = nullptr;
		constexpr TypeID typeID = Scene::GetTypeID<Comp>();

		// Ensure that the component type has been registered before we proceed
		if (const auto& typeInfoIt = Scene::GetTypeInfo().find(typeID); typeInfoIt != Scene::GetTypeInfo().end())
			typeInfo = &typeInfoIt->second;
		else
			typeInfo = &Scene::RegisterType<Comp>();

		// Return early in case component is already reflected
		auto& reflCompInfoMap = Scene::s_ReflectedCompInfo;
		if (const auto& it = reflCompInfoMap.find(typeID); it != reflCompInfoMap.end())
		{
			CRITICAL("Attempted to reflect component '{}', but this operation has already been completed. Reflecting a component more than once is not allowed.", typeInfo->debugName);
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
		if (const auto& typeInfoIt = Scene::GetTypeInfo().find(varTypeID); typeInfoIt == Scene::GetTypeInfo().end())
			Scene::RegisterType<Var>();

		// Check if this variable has already been reflected
		auto& memberVec = reflCompInfoIt->second.members;
		size_t byteOffset = reinterpret_cast<size_t>(&(reinterpret_cast<Comp*>(0)->*var));
		auto varIt = std::find_if(memberVec.begin(), memberVec.end(), [&byteOffset](const ReflectionCompInfo::MemberInfo& var) { return var.byteOffset == byteOffset; });

		if (varIt != memberVec.end())
		{
			WARNING("Attempted to register variable {} again.", typeid(var).name());
			return VariableReflectionSetup(*varIt);
		}

		ReflectionCompInfo::MemberInfo& memberInfo = memberVec.emplace_back(ReflectionCompInfo::MemberInfo());
		memberInfo.props.displayName = typeid(Var).name();
		memberInfo.typeID = varTypeID;
		memberInfo.byteOffset = byteOffset;

		return VariableReflectionSetup(memberInfo);
	}

	template<typename... Args, size_t... Indices>
	void LAG::SceneReflect::FuncAdapter(void (*func)(Args...), const std::vector<std::any>& args, std::index_sequence<Indices...>)
	{
#ifdef DEBUG
		([&]
		{
			auto& anyType = args[Indices];
			if (anyType.type() != typeid(Args))
				CRITICAL("Mismatch between type ({}) and argument ({}).", typeid(Args).name(), anyType.type().name());
		}(), ...);
#endif
		func(std::any_cast<Args>(args[Indices])...);
	}

	template<typename Type, auto Func>
	inline void LAG::SceneReflect::RegisterFunc(Hash64 funcNameID)
	{
		// Ensure that the variable type has been registered before we proceed
		TypeInfo* typeInfo = nullptr;
		if (const auto& typeInfoIt = Scene::GetTypeInfo().find(Scene::GetTypeID<Type>()); typeInfoIt != Scene::GetTypeInfo().end())
			typeInfo = &typeInfoIt->second;
		else 
			typeInfo = &Scene::RegisterType<Type>();

		auto& funcMap = typeInfo->funcs;
		funcMap[funcNameID] = [](const std::vector<std::any>& args)
			{
				constexpr size_t ArgsCount = FuncTraits<decltype(Func)>::ArgsCount;
				if (ArgsCount != args.size())
					LAG_ASSERT("Cannot run function: argument count mismatch.");

				FuncAdapter(Func, args, std::make_index_sequence<ArgsCount>());
			};
	}

	template<typename Comp>
	inline ReflectionCompInfo::Properties* SceneReflect::GetComponentProps()
	{
		return GetComponentProps(Scene::GetTypeID<Comp>());
	}
}
