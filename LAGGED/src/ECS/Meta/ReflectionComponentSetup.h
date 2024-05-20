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
			constexpr entt::hashed_string VISIBLE_IN_EDITOR = entt::hashed_string("VISIBLE_IN_EDITOR");
		}

		namespace VariableProperties
		{
			constexpr entt::hashed_string DISPLAY_NAME = entt::hashed_string("DISPLAY_NAME");
			constexpr entt::hashed_string VISIBLE_IN_EDITOR = entt::hashed_string("VISIBLE_IN_EDITOR");
			constexpr entt::hashed_string READ_ONLY = entt::hashed_string("READ_ONLY");
		}

		template<typename ClassType>
		class ComponentReflectionSetup
		{
			friend class ReflectionSystem;
		public:
			ComponentReflectionSetup(const ComponentReflectionSetup& other); //Copy constructor
			ComponentReflectionSetup& operator=(const ComponentReflectionSetup& other); //Copy assignment operator
			~ComponentReflectionSetup();

			ComponentReflectionSetup& SetDisplayName(const std::string& displayName);
			ComponentReflectionSetup& SetVisibleInEditor(bool isVisible);

		private:
			ComponentReflectionSetup()
			{
				static_assert(!std::is_fundamental<ClassType>::value, "Type cannot be a fundamental type.");

				m_Factory = new entt::meta_factory<ClassType>(entt::meta<ClassType>());
				std::string displayName = typeid(ClassType).name();
				m_Factory->type(entt::hashed_string(displayName.c_str()));
			}

			entt::meta_factory<ClassType>* m_Factory = nullptr;
		};

		//TODO: Figure out a way of doing this without having to pass in the ClassType. Should make the syntax a bit nicer to look at.
		template<typename ClassType, auto Variable>
		class VariableReflectionSetup
		{
			friend class ReflectionSystem;
		public:
			VariableReflectionSetup(const VariableReflectionSetup& other); //Copy constructor
			VariableReflectionSetup& operator=(const VariableReflectionSetup& other); //Copy assignment operator
			~VariableReflectionSetup();

			VariableReflectionSetup& SetDisplayName(const std::string& displayName);
			VariableReflectionSetup& SetCallbackFunction();
			VariableReflectionSetup& SetReadOnly(bool isReadOnly);

		private:
			VariableReflectionSetup()
			{
				static_assert(std::is_member_object_pointer<decltype(Variable)>::value, "Type needs to be a non-static member object pointer.");

				m_Factory = new entt::meta_factory<ClassType>(entt::meta<ClassType>());
				m_Factory->data<Variable>(entt::hashed_string(typeid(ClassType).name()));
			}

			entt::meta_factory<ClassType>* m_Factory = nullptr;
		};

		class ReflectionSystem
		{
		public:
			template<typename T>
			ComponentReflectionSetup<T> RegisterComponent() { return ComponentReflectionSetup<T>(); };

			template<typename ClassType, auto Variable>
			VariableReflectionSetup<ClassType, Variable> RegisterVariable() { return VariableReflectionSetup<ClassType, Variable>(); };
		};



		//////////////////////////////////////////////////////////////////
		// Function implementations for the component reflection system //
		//////////////////////////////////////////////////////////////////

		template<typename ClassType>
		inline ComponentReflectionSetup<ClassType>::ComponentReflectionSetup(const ComponentReflectionSetup& other) :
			m_Factory(other.m_Factory)
		{}

		template<typename ClassType>
		inline ComponentReflectionSetup<ClassType>& ComponentReflectionSetup<ClassType>::operator=(const ComponentReflectionSetup<ClassType>& other)
		{
			return *this = ComponentReflectionSetup(other);
		}

		template<typename ClassType>
		inline ComponentReflectionSetup<ClassType>::~ComponentReflectionSetup()
		{
			delete m_Factory;
			m_Factory = nullptr;
		}

		template<typename ClassType>
		inline ComponentReflectionSetup<ClassType>& ComponentReflectionSetup<ClassType>::SetDisplayName(const std::string& displayName)
		{
			m_Factory->prop(ComponentProperties::DISPLAY_NAME, displayName); 
			return *this;
		}

		template<typename ClassType>
		inline ComponentReflectionSetup<ClassType>& ComponentReflectionSetup<ClassType>::SetVisibleInEditor(bool isVisible)
		{
			m_Factory->prop(ComponentProperties::VISIBLE_IN_EDITOR, false); 
			return *this;
		}



		/////////////////////////////////////////////////////////////////
		// Function implementations for the variable reflection system //
		/////////////////////////////////////////////////////////////////

		//template<typename ClassType, auto Variable>
		//inline VariableReflectionSetup<ClassType, Variable>::VariableReflectionSetup(const VariableReflectionSetup& other) :
		//	m_Factory(other.m_Factory)
		//{}

		template<typename ClassType, auto Variable>
		inline VariableReflectionSetup<ClassType, Variable>::VariableReflectionSetup(const VariableReflectionSetup& other) :
			m_Factory(other.m_Factory)
		{}

		template<typename ClassType, auto Variable>
		inline VariableReflectionSetup<ClassType, Variable>& VariableReflectionSetup<ClassType, Variable>::operator=(const VariableReflectionSetup& other)
		{
			return *this = VariableReflectionSetup(other);
		}

		template<typename ClassType, auto Variable>
		inline VariableReflectionSetup<ClassType, Variable>::~VariableReflectionSetup()
		{
			delete m_Factory;
			m_Factory = nullptr;
		}

		template<typename ClassType, auto Variable>
		inline VariableReflectionSetup<ClassType, Variable>& VariableReflectionSetup<ClassType, Variable>::SetDisplayName(const std::string& displayName)
		{
			m_Factory->prop(VariableProperties::DISPLAY_NAME, displayName);
			return *this;
		}

		template<typename ClassType, auto Variable>
		inline VariableReflectionSetup<ClassType, Variable> & VariableReflectionSetup<ClassType, Variable>::SetCallbackFunction()
		{
			LAG_ASSERT("TODO: Implement"); return *this;
		}

		template<typename ClassType, auto Variable>
		inline VariableReflectionSetup<ClassType, Variable> & VariableReflectionSetup<ClassType, Variable>::SetReadOnly(bool isReadOnly)
		{
			LAG_ASSERT("TODO: Implement"); return *this;
		}

};
}