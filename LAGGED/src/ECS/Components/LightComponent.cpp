#include "LightComponent.h"

#include "ECS/Scene.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
    bool LightComponent::InitializeReflection()
    {
        SceneReflect& refl = SceneReflect::Get();
        refl.ReflectComponent<LightComponent>().SetDisplayName("Light Component");

        refl.ReflectVariable(&LightComponent::lightColor).SetDisplayName("Color");
        refl.ReflectVariable(&LightComponent::lightIntensity).SetDisplayName("Intensity");
        refl.ReflectVariable(&LightComponent::lightAttenuation).SetDisplayName("Attenuation");

        return true;
    }
}