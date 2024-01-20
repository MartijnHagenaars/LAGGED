#include "Precomp.h"
#include "CameraComponent.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Meta/MetaDefines.h"

int LAG::CameraComponent::InitReflection()
{
    auto factory = entt::meta<CameraComponent>();
    factory.type(entt::type_hash<CameraComponent>::value());
    factory.data<&CameraComponent::fov>(entt::hashed_string("fov")).prop(Reflection::DISPLAY_NAME, std::string("FOV"));
    factory.data<&CameraComponent::movementSpeed>(entt::hashed_string("movementSpeed")).prop(Reflection::DISPLAY_NAME, std::string("Movement speed"));
    factory.data<&CameraComponent::rotationSpeed>(entt::hashed_string("rotationSpeed")).prop(Reflection::DISPLAY_NAME, std::string("Rotation speed"));

    return 0;
}
