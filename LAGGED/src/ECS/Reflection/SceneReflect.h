#pragma once
#include "ECS/Scene.h"

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
		static ComponentReflectionSetup RegisterComponent();

		template <typename Comp, typename Var>
		static VariableReflectionSetup RegisterVariable(Var Comp::* var);

		struct ComponentVariableProperties
		{
			size_t byteOffset = -1;

			// PROPERTIES

			bool isHidden = true;
			bool isReadOnly = false;

			std::string displayName;
		};

		struct ComponentClassProperties
		{
			std::vector<ComponentVariableProperties> varProperties;

			// PROPERTIES

			bool isHidden = true;
			std::string displayName;
		};

	private:
		inline static std::unordered_map<ComponentID, ComponentClassProperties> m_ComponentProperties = {};
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
		ComponentReflectionSetup(SceneReflect::ComponentClassProperties& compProperties);

		SceneReflect::ComponentClassProperties& m_Props;
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
		VariableReflectionSetup(SceneReflect::ComponentVariableProperties& varProperties);

		SceneReflect::ComponentVariableProperties& m_Props;
	};

}

#include "SceneReflect.inl"