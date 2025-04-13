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

		struct ComponentVariableData
		{
			size_t byteOffset = -1;

			struct Properties
			{
				bool isHidden = true;
				bool isReadOnly = false;
				std::string displayName;
			} properties;
		};

		struct ComponentClassData
		{
			std::vector<ComponentVariableData> vars;

			struct Properties
			{
				bool isHidden = true;
				std::string displayName;
			} properties;
		};

	private:
		// Returns the map containing component properties
		// Done through a getter function to avoid Static Initialization Order Fiasco
		static std::unordered_map<ComponentID, SceneReflect::ComponentClassData>& GetComponentProperties();
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
		ComponentReflectionSetup(SceneReflect::ComponentClassData& compProperties);

		SceneReflect::ComponentClassData& m_Data;
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
		VariableReflectionSetup(SceneReflect::ComponentVariableData& vars);

		SceneReflect::ComponentVariableData& m_Data;
	};

}

#include "SceneReflect.inl"