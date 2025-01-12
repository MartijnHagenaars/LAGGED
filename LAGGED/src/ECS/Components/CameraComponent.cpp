#include "CameraComponent.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"

bool LAG::CameraComponent::InitializeReflection()
{
    //Reflection::ReflectionSystem<CameraComponent> setup;
    //setup.RegisterComponent().SetDisplayName("Camera Component").SetVisibleInEditor(true);

    //setup.RegisterVariable<&CameraComponent::isActive>().SetDisplayName("Is active?");
    //setup.RegisterVariable<&CameraComponent::fov>().SetDisplayName("FOV");
    //setup.RegisterVariable<&CameraComponent::movementSpeed>().SetDisplayName("Movement speed");
    //setup.RegisterVariable<&CameraComponent::rotationSpeed>().SetDisplayName("Rotation speed");

    return true;
}
