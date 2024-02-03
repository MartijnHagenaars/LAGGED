#include "TerrainComponents.h"

#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionDefines.h"

namespace LAG
{
	bool PlaneComponent::InitializeReflection()
	{
        auto factory = entt::meta<PlaneComponent>();
        factory.type(entt::type_hash<PlaneComponent>::value());

        factory.data<&PlaneComponent::m_Surface>(entt::hashed_string("m_Surface")).prop(Reflection::DISPLAY_NAME, std::string("Surface"));
        factory.data<&PlaneComponent::m_Texture>(entt::hashed_string("m_Texture")).prop(Reflection::DISPLAY_NAME, std::string("Texture"));

        return false;
	}

}