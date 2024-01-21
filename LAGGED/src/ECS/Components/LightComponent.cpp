#include "Precomp.h"
#include "LightComponent.h"

#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionDefines.h"

bool LAG::LightComponent::InitializeReflection()
{
    auto factory = entt::meta<LightComponent>();
    factory.type(entt::type_hash<LightComponent>::value());

    factory.data<&LightComponent::lightColor>(entt::hashed_string("lightColor")).prop(Reflection::DISPLAY_NAME, std::string("Color"));
    factory.data<&LightComponent::lightIntensity>(entt::hashed_string("lightIntensity")).prop(Reflection::DISPLAY_NAME, std::string("Intensity"));
    factory.data<&LightComponent::lightAttenuation>(entt::hashed_string("lightAttenuation")).prop(Reflection::DISPLAY_NAME, std::string("Attenuation"));

    return false;
}
