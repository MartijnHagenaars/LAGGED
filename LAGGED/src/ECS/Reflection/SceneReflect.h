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
		friend class Entity;
	public:
		~SceneReflect() = default;

		static SceneReflect& Get();

		template<typename Comp>
		ComponentReflectionSetup Register();

		template <typename Comp, typename Var>
		VariableReflectionSetup RegisterVariable(Var Comp::* var);

		struct VariableReflectionData
		{
			size_t byteOffset = -1;

			struct Properties
			{
				bool isHidden = false;
				bool isReadOnly = false;
				std::string displayName;
			} props;
		};

		struct ComponentReflectionData
		{
			std::vector<VariableReflectionData> vars;

			struct Properties
			{
				bool isHidden = false;
				std::string displayName;
			} props;
		};

	private:
		SceneReflect() = default;

		std::unordered_map<ComponentID, SceneReflect::ComponentReflectionData>& GetComponentProperties();

	private:
		// Returns the map containing component properties
		// Done through a getter function to avoid Static Initialization Order Fiasco
		std::unordered_map<ComponentID, SceneReflect::ComponentReflectionData> m_CompProperties;
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
		ComponentReflectionSetup(SceneReflect::ComponentReflectionData& compProperties);

		SceneReflect::ComponentReflectionData& m_Data;
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
		VariableReflectionSetup(SceneReflect::VariableReflectionData& vars);

		SceneReflect::VariableReflectionData& m_Data;
	};

}

#include "SceneReflect.inl"