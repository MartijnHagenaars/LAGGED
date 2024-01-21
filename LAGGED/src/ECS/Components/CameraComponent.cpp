#include "Precomp.h"
#include "CameraComponent.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionDefines.h"

bool LAG::CameraComponent::InitializeReflection()
{
    auto factory = entt::meta<CameraComponent>();
    factory.type(entt::type_hash<CameraComponent>::value());

    factory.data<&CameraComponent::isActive>(entt::hashed_string("isActive")).prop(Reflection::DISPLAY_NAME, std::string("Active"));
    factory.data<&CameraComponent::fov>(entt::hashed_string("fov")).prop(Reflection::DISPLAY_NAME, std::string("FOV"));
    factory.data<&CameraComponent::movementSpeed>(entt::hashed_string("movementSpeed")).prop(Reflection::DISPLAY_NAME, std::string("Movement speed"));
    factory.data<&CameraComponent::rotationSpeed>(entt::hashed_string("rotationSpeed")).prop(Reflection::DISPLAY_NAME, std::string("Rotation speed"));

    return 0;
}
