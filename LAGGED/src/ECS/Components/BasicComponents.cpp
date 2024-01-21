#include "Precomp.h"
#include "BasicComponents.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionDefines.h"

bool LAG::DefaultComponent::InitializeReflection()
{
    auto factory = entt::meta<DefaultComponent>();
    factory.type(entt::type_hash<DefaultComponent>::value());

    factory.data<&DefaultComponent::name>(entt::hashed_string("name")).prop(Reflection::DISPLAY_NAME, std::string("Name"));

    return false; 
}

bool LAG::TransformComponent::InitializeReflection()
{
    auto factory = entt::meta<TransformComponent>();
    factory.type(entt::type_hash<TransformComponent>::value());

    factory.data<&TransformComponent::position>(entt::hashed_string("position")).prop(Reflection::DISPLAY_NAME, std::string("Position"));
    factory.data<&TransformComponent::rotation>(entt::hashed_string("rotation")).prop(Reflection::DISPLAY_NAME, std::string("Rotation"));
    factory.data<&TransformComponent::scale>(entt::hashed_string("scale")).prop(Reflection::DISPLAY_NAME, std::string("Scale"));

    return false;
}
