#include "LightComponent.h"

#include "ECS/Scene.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
    bool LightComponent::InitializeReflection()
    {
        SceneReflect& refl = SceneReflect::Get();
        refl.Register<LightComponent>().SetDisplayName("Light Component");

        refl.RegisterVariable(&LightComponent::lightColor).SetDisplayName("Color");
        refl.RegisterVariable(&LightComponent::lightIntensity).SetDisplayName("Intensity");
        refl.RegisterVariable(&LightComponent::lightAttenuation).SetDisplayName("Attenuation");

        return true;
    }
}