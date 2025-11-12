#pragma once
#include "ECS/Scene.h"
#include "ECS/Containers.h"

#include <unordered_map>
#include <memory>

namespace LAG
{
	class ComponentReflectionSetup;
	class VariableReflectionSetup;

	class SceneReflect
	{
	public:
		template<typename Comp>
		static ComponentReflectionSetup ReflectComponent();

		template <typename Comp, typename Var>
		static VariableReflectionSetup ReflectVariable(Var Comp::* var);

		template <typename T, auto Func>
		static void RegisterFunc(Hash64 funcNameID);

		template<typename Comp>
		static ReflectionCompInfo::Properties* GetComponentProps();
		static ReflectionCompInfo::Properties* GetComponentProps(TypeID id);

	private:
		SceneReflect() = delete;


		/////////////////////////////////////////////////////////////////////////
		// Helper classes and functions for registering (reflection) functions //
		/////////////////////////////////////////////////////////////////////////

		template <typename T> struct FuncTraits;

		template <typename... Args>
		struct FuncTraits<void(*)(Args...)>
		{
			static constexpr std::size_t ArgsCount = sizeof...(Args);
		};

		template<typename... Args, size_t... Indices>
		static void FuncAdapter(void (*func)(Args...), const std::vector<std::any>& args, std::index_sequence<Indices...>);
	};

	class ComponentReflectionSetup
	{
		friend class SceneReflect;
	public:
		/// <summary>
		/// Set the display name of the component in the editor.
		/// </summary>
		/// <param name="displayName">Name that is displayed in the editor.</param>
		/// <returns>The current instance for chaining further configuration.</returns>
		ComponentReflectionSetup& SetDisplayName(const std::string& displayName);

		/// <summary>
		/// Determines whether the component should be visible in the editor.
		/// </summary>
		/// <param name="isVisible">When set to true, the component is hidden in the editor.</param>
		/// <returns>The current instance for chaining further configuration.</returns>
		ComponentReflectionSetup& SetHidden(bool isHidden);

	private:
		ComponentReflectionSetup() = delete;
		ComponentReflectionSetup(ReflectionCompInfo& info);

		ReflectionCompInfo& m_Info;
	};

	class VariableReflectionSetup
	{
		friend class SceneReflect;
	public:
		/// <summary>
		/// Set the display name of the variable in the editor.
		/// </summary>
		/// <param name="displayName">Name that is displayed in the editor.</param>
		/// <returns>The current instance for chaining further configuration.</returns>
		VariableReflectionSetup& SetDisplayName(const std::string& displayName);

		/// <summary>
		/// Determines whether the variable should be displayed in the editor.
		/// </summary>
		/// <param name="displayName">When set to true, the variable is hidden in the editor.</param>
		/// <returns>The current instance for chaining further configuration.</returns>
		VariableReflectionSetup& SetHidden(bool isHidden);

		/// <summary>
		/// Sets the variable's read-only status.
		/// </summary>
		/// <param name="isReadOnly">When set to true, the variable cannot be changed in the editor.</param>
		/// <returns>The current instance for chaining further configuration.</returns>
		VariableReflectionSetup& SetReadOnly(bool isReadOnly);

	private:
		VariableReflectionSetup() = delete;
		VariableReflectionSetup(ReflectionCompInfo::MemberInfo& info);

		ReflectionCompInfo::MemberInfo& m_Info;
	};

}

#include "SceneReflect.inl"