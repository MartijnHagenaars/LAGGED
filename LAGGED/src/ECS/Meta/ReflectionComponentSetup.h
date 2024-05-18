#pragma once
#include "entt/entt.hpp"
#include "ReflectionTypeDefinitions.h"
#include "ECS/Components/BasicComponents.h"
#include "Core/Defines.h"

namespace LAG
{
	namespace Reflection
	{
		constexpr entt::hashed_string DISPLAY_NAME = entt::hashed_string("DISPLAY_NAME");

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


	};
}