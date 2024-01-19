#include "Precomp.h"
#include "CameraComponent.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"

int LAG::CameraComponent::InitReflection()
{
    auto factory = entt::meta<CameraComponent>();
    factory.type(entt::type_hash<CameraComponent>::value());
    factory.data<&CameraComponent::fov>(entt::hashed_string("fov")).prop(entt::hashed_string("DisplayName"), std::string("FOV"));
    factory.data<&CameraComponent::movementSpeed>(entt::hashed_string("movementSpeed")).prop(entt::hashed_string("DisplayName"), std::string("Movement speed"));
    factory.data<&CameraComponent::rotationSpeed>(entt::hashed_string("rotationSpeed")).prop(entt::hashed_string("DisplayName"), std::string("Rotation speed"));

    return 0;
}
