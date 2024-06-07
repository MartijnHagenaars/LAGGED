#pragma once
#include "entt/entt.hpp"
#include "ReflectionTypeDefinitions.h"
#include "ECS/Components/BasicComponents.h"
#include "Core/Defines.h"

namespace LAG
{
	namespace Reflection
	{
		namespace ComponentProperties
		{
			constexpr entt::hashed_string DISPLAY_NAME = entt::hashed_string("DISPLAY_NAME");

			//TODO: Implement this one!
			constexpr entt::hashed_string HIDE_IN_EDITOR = entt::hashed_string("HIDE_IN_EDITOR");
		}

		namespace VariableProperties
		{
			constexpr entt::hashed_string DISPLAY_NAME = entt::hashed_string("DISPLAY_NAME");
			constexpr entt::hashed_string HIDE_IN_EDITOR = entt::hashed_string("HIDE_IN_EDITOR");

			//TODO: Implement this one!
			constexpr entt::hashed_string READ_ONLY = entt::hashed_string("READ_ONLY");
		}

		//Forward declaring reflection system class
		template<typename ClassType>
		class ReflectionSystem; 

		template<typename ClassType>
		class ComponentReflectionSetup
		{
			friend class ReflectionSystem<ClassType>;
		public:
			/// <summary>
			/// Set the display name of the component in the editor.
			/// </summary>
			/// <param name="displayName">Name that is displayed in the editor.</param>
			/// <returns>The current instance for chaining further configuration.</returns>
			ComponentReflectionSetup& SetDisplayName(const std::string& displayName);

			/// <summary>
			/// Set whether the component is visible in the editor.
			/// </summary>
			/// <param name="isVisible">When set to true, the component is visible in the editor.</param>
			/// <returns>The current instance for chaining further configuration.</returns>
			ComponentReflectionSetup& SetVisibleInEditor(bool isVisible);

		private:
			ComponentReflectionSetup(entt::meta_factory<ClassType>& factory) : 
				m_Factory(factory)
			{
				static_assert(!std::is_fundamental<ClassType>::value, "Type cannot be a fundamental type.");

				std::string displayName = typeid(ClassType).name();
				m_Factory.type(entt::hashed_string(displayName.c_str()));
				
				m_Factory.prop(entt::hashed_string("REFLECTED_COMPONENT"), displayName);
			}

			entt::meta_factory<ClassType>& m_Factory;
		};

		//TODO: Figure out a way of doing this without having to pass in the ClassType. Should make the syntax a bit nicer to look at.
		template<typename ClassType, auto Variable>
		class VariableReflectionSetup
		{
			friend class ReflectionSystem<ClassType>;
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

			//TODO: Needs to be implemented.
			VariableReflectionSetup& SetCallbackFunction();

			/// <summary>
			/// Sets the variable's read-only status.
			/// </summary>
			/// <param name="isReadOnly">When set to true, the variable cannot be changed in the editor.</param>
			/// <returns>The current instance for chaining further configuration.</returns>
			VariableReflectionSetup& SetReadOnly(bool isReadOnly);

		private:
			VariableReflectionSetup(entt::meta_factory<ClassType>& factory) :
				m_Factory(factory)
			{
				static_assert(std::is_member_object_pointer<decltype(Variable)>::value, "Type needs to be a non-static member object pointer.");

				//Get a count of how many variables are attached to a component

				entt::meta_type type = entt::resolve<ClassType>();
				if (type)
				{
					std::size_t variableCount = 0;
					for (const auto& it : type.data())
						++variableCount;

					//Here, we create a unique string and register the variable.
					std::string typeName = std::string(typeid(Variable).name()) + "_" + std::to_string(variableCount);
					m_Factory.data<Variable>(entt::hashed_string(typeName.c_str()));
				}
				else Logger::Critical("Tried to setup a variable for a component that hasn't been registered.");
			}

			entt::meta_factory<ClassType>& m_Factory;
		};

		template<typename ClassType>
		class ReflectionSystem
		{
		public:
			ReflectionSystem()
			{
			}

			~ReflectionSystem()
			{
			}

			//TODO: Rule of three

			/// <summary>
			/// Registers a component for use in the editor.
			/// </summary>
			/// <typeparam name="T">The component type</typeparam>
			/// <returns>The class for configuring the reflection of the component. Uses the builder pattern.</returns>
			ComponentReflectionSetup<ClassType> RegisterComponent() { return ComponentReflectionSetup<ClassType>(m_Factory); }

			/// <summary>
			/// Registers a variable from a specific component. Registering the variable allows it to be used in the editor.
			/// </summary>
			/// <typeparam name="ClassType">The component type.</typeparam>
			/// <typeparam name="Variable">The variable / member function pointer.</typeparam>
			/// <returns>The class for configuring the reflection of the variable. Uses the builder pattern.</returns>
			template<auto Variable>
			VariableReflectionSetup<ClassType, Variable> RegisterVariable() { return VariableReflectionSetup<ClassType, Variable>(m_Factory); };

		private:
			entt::meta_factory<ClassType> m_Factory = entt::meta<ClassType>();
		};



		//////////////////////////////////////////////////////////////////
		// Function implementations for the component reflection system //
		//////////////////////////////////////////////////////////////////

		template<typename ClassType>
		inline ComponentReflectionSetup<ClassType>& ComponentReflectionSetup<ClassType>::SetDisplayName(const std::string& displayName)
		{
			m_Factory.prop(ComponentProperties::DISPLAY_NAME, displayName);
			return *this;
		}

		template<typename ClassType>
		inline ComponentReflectionSetup<ClassType>& ComponentReflectionSetup<ClassType>::SetVisibleInEditor(bool isVisible)
		{
			m_Factory.prop(ComponentProperties::HIDE_IN_EDITOR, false);
			return *this;
		}



		/////////////////////////////////////////////////////////////////
		// Function implementations for the variable reflection system //
		/////////////////////////////////////////////////////////////////

		template<typename ClassType, auto Variable>
		inline VariableReflectionSetup<ClassType, Variable>& VariableReflectionSetup<ClassType, Variable>::SetDisplayName(const std::string& displayName)
		{
			m_Factory.prop(VariableProperties::DISPLAY_NAME, displayName);
			return *this;
		}

		template<typename ClassType, auto Variable>
		inline VariableReflectionSetup<ClassType, Variable>& VariableReflectionSetup<ClassType, Variable>::SetHidden(bool isHidden)
		{
			m_Factory.prop(VariableProperties::HIDE_IN_EDITOR, isHidden);
			return *this;
		}

		template<typename ClassType, auto Variable>
		inline VariableReflectionSetup<ClassType, Variable>& VariableReflectionSetup<ClassType, Variable>::SetCallbackFunction()
		{
			LAG_ASSERT("TODO: Implement"); return *this;
		}

		template<typename ClassType, auto Variable>
		inline VariableReflectionSetup<ClassType, Variable>& VariableReflectionSetup<ClassType, Variable>::SetReadOnly(bool isReadOnly)
		{
			LAG_ASSERT("TODO: Implement"); return *this;
		}

	};
}