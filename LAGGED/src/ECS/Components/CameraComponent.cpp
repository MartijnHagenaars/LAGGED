#include "CameraComponent.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
    bool CameraComponent::InitializeReflection()
    {
        SceneReflect refl;
        refl.RegisterComponent<CameraComponent>().SetDisplayName("Camera Component");

        refl.RegisterVariable(&CameraComponent::isActive).SetDisplayName("Is active?");
        refl.RegisterVariable(&CameraComponent::fov).SetDisplayName("FOV");
        refl.RegisterVariable(&CameraComponent::movementSpeed).SetDisplayName("Movement speed");
        refl.RegisterVariable(&CameraComponent::rotationSpeed).SetDisplayName("Rotation speed");

        return true;
    }
}
