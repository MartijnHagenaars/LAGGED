#include "LightComponent.h"

#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionComponentSetup.h"

bool LAG::LightComponent::InitializeReflection()
{
    auto factory = entt::meta<LightComponent>();
    factory.type(entt::type_hash<LightComponent>::value()).prop(Reflection::VariableProperties::DISPLAY_NAME, std::string("Light Component"));;

    factory.data<&LightComponent::lightColor>(entt::hashed_string("lightColor")).prop(Reflection::VariableProperties::DISPLAY_NAME, std::string("Color"));
    factory.data<&LightComponent::lightIntensity>(entt::hashed_string("lightIntensity")).prop(Reflection::VariableProperties::DISPLAY_NAME, std::string("Intensity"));
    factory.data<&LightComponent::lightAttenuation>(entt::hashed_string("lightAttenuation")).prop(Reflection::VariableProperties::DISPLAY_NAME, std::string("Attenuation"));

    return false;
}
