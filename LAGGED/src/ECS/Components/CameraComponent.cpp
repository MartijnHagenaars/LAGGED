#include "CameraComponent.h"

#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
    bool CameraComponent::InitializeReflection()
    {
        SceneReflect::ReflectComponent<CameraComponent>().SetDisplayName("Camera Component");

        SceneReflect::ReflectVariable(&CameraComponent::isActive).SetDisplayName("Is active?");
        SceneReflect::ReflectVariable(&CameraComponent::fov).SetDisplayName("FOV");
        SceneReflect::ReflectVariable(&CameraComponent::movementSpeed).SetDisplayName("Movement speed");
        SceneReflect::ReflectVariable(&CameraComponent::rotationSpeed).SetDisplayName("Rotation speed");

        return true;
    }
}
