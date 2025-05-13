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
		ComponentReflectionSetup RegisterComponent();

		template <typename Comp, typename Var>
		VariableReflectionSetup RegisterVariable(Var Comp::* var);

		struct VariableData
		{
			size_t byteOffset = -1;

			struct Properties
			{
				bool isHidden = false;
				bool isReadOnly = false;
				std::string displayName;
			} props;
		};

		struct ComponentData
		{
			std::vector<VariableData> vars;

			struct Properties
			{
				bool isHidden = false;
				std::string displayName;
			} props;
		};

	public:
		//FIXME: Fully implement iterator...
		struct Iterator
		{
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;
			using InnerIterator = std::unordered_map<ComponentID, SceneReflect::ComponentData>::iterator;

			Iterator(InnerIterator pIt, SceneReflect* pParent);

			SceneReflect::ComponentData* operator*() const;
			SceneReflect::ComponentData& operator->() const;

			Iterator& operator++();
			Iterator operator++(int);
			Iterator& operator--();
			Iterator operator--(int);

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_pIt == b.m_pIt; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_pIt != b.m_pIt; }

		private:
			InnerIterator m_pIt;
			SceneReflect* m_pParent = nullptr;
		};

		Iterator begin();
		Iterator end();

	private:
		SceneReflect() = default;

		std::unordered_map<ComponentID, SceneReflect::ComponentData>& GetComponentProperties();

	private:
		// Returns the map containing component properties
		// Done through a getter function to avoid Static Initialization Order Fiasco
		std::unordered_map<ComponentID, SceneReflect::ComponentData> m_CompProperties;
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
		ComponentReflectionSetup(SceneReflect::ComponentData& compProperties);

		SceneReflect::ComponentData& m_Data;
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
		VariableReflectionSetup(SceneReflect::VariableData& vars);

		SceneReflect::VariableData& m_Data;
	};

}

#include "SceneReflect.inl"