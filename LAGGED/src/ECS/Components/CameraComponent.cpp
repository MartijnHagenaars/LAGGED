#include "CameraComponent.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
    bool CameraComponent::InitializeReflection()
    {
        SceneReflect& refl = SceneReflect::Get();
        refl.ReflectComponent<CameraComponent>().SetDisplayName("Camera Component");

        refl.ReflectVariable(&CameraComponent::isActive).SetDisplayName("Is active?");
        refl.ReflectVariable(&CameraComponent::fov).SetDisplayName("FOV");
        refl.ReflectVariable(&CameraComponent::movementSpeed).SetDisplayName("Movement speed");
        refl.ReflectVariable(&CameraComponent::rotationSpeed).SetDisplayName("Rotation speed");

        return true;
    }
}
