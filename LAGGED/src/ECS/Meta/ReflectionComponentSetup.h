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
		}

		template<typename ClassType>
		class ComponentReflectionSetup
		{
			friend class ReflectionSystem;
		public:
			// Copy constructor
			ComponentReflectionSetup(const ComponentReflectionSetup& other);

			// Copy assignment operator
			ComponentReflectionSetup& operator=(const ComponentReflectionSetup& other);

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

		template<auto T>
		class VariableReflectionSetup
		{
			friend class ReflectionSystem;
		public:
			VariableReflectionSetup& SetDisplayName(const std::string& displayName) { LAG_ASSERT("TODO: Implement"); return *this; }
			VariableReflectionSetup& SetCallbackFunction() { LAG_ASSERT("TODO: Implement"); return *this; }

		private:
			VariableReflectionSetup()
			{
				static_assert(std::is_member_object_pointer<decltype(T)>::value, "Type needs to be a non-static member object pointer.");

				// Extract ClassType from the member pointer T
				using MemberPointerType = decltype(T);
				using ClassType = typename std::remove_pointer_t<MemberPointerType>::class_type;

				// Register the member variable using entt::meta
				auto factory = entt::meta<ClassType>();
				factory.template data<T>("name");
			}
		};

		class ReflectionSystem
		{
		public:
			template<typename T>
			ComponentReflectionSetup<T> RegisterComponent() { return ComponentReflectionSetup<T>(); };

			template<auto T>
			VariableReflectionSetup<T> RegisterVariable() { return VariableReflectionSetup<T>(); };
		};


		//#define LAG_REGISTER_COMPONENT(componentType)									\
		//		bool Setup##componentType()												\
		//	{																			\
		//		printf("Hello from %s function!\n", #componentType);					\
		//		return true;															\
		//	}																			\
		//		bool Setup##componentType##Success = Setup##componentType();			\
		//
		//#define LAG_REGISTER_VARIABLE(variable)*/
		//


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

	};
}