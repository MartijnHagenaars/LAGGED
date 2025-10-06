#include "LightComponent.h"

#include "ECS/Scene.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
    bool LightComponent::InitializeReflection()
    {
        SceneReflect::ReflectComponent<LightComponent>().SetDisplayName("Light Component");

        SceneReflect::ReflectVariable(&LightComponent::lightColor).SetDisplayName("Color");
        SceneReflect::ReflectVariable(&LightComponent::lightIntensity).SetDisplayName("Intensity");
        SceneReflect::ReflectVariable(&LightComponent::lightAttenuation).SetDisplayName("Attenuation");

        return true;
    }
}