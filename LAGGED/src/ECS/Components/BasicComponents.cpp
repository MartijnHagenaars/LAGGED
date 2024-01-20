#include "Precomp.h"
#include "BasicComponents.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Meta/MetaDefines.h"

int LAG::DefaultComponent::InitReflection()
{
    auto factory = entt::meta<DefaultComponent>();
    factory.type(entt::type_hash<DefaultComponent>::value());
    factory.data<&DefaultComponent::name>(entt::hashed_string("name")).prop(Reflection::DISPLAY_NAME, std::string("Name"));

    return 0; 
}

int LAG::TransformComponent::InitReflection()
{
    auto factory = entt::meta<TransformComponent>();
    factory.type(entt::type_hash<TransformComponent>::value());
    factory.data<&TransformComponent::position>(entt::hashed_string("position")).prop(Reflection::DISPLAY_NAME, std::string("Position"));
    factory.data<&TransformComponent::rotation>(entt::hashed_string("rotation")).prop(Reflection::DISPLAY_NAME, std::string("Rotation"));
    factory.data<&TransformComponent::scale>(entt::hashed_string("scale")).prop(Reflection::DISPLAY_NAME, std::string("Scale"));

    return 0;
}
