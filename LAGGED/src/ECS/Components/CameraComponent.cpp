#include "CameraComponent.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionComponentSetup.h"

bool LAG::CameraComponent::InitializeReflection()
{
    Reflection::ReflectionSystem setup;
    setup.RegisterComponent<CameraComponent>().SetDisplayName("Camera Component").SetVisibleInEditor(true);
    setup.RegisterVariable<CameraComponent, &CameraComponent::isActive>().SetDisplayName("Is active?");
    setup.RegisterVariable<CameraComponent, &CameraComponent::fov>().SetDisplayName("FOV");
    setup.RegisterVariable<CameraComponent, &CameraComponent::movementSpeed>().SetDisplayName("Movement speed");
    setup.RegisterVariable<CameraComponent, &CameraComponent::rotationSpeed>().SetDisplayName("Rotation speed");

    return true;
}
