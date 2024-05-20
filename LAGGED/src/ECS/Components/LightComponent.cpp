#include "LightComponent.h"

#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionComponentSetup.h"

bool LAG::LightComponent::InitializeReflection()
{
    Reflection::ReflectionSystem setup;
    setup.RegisterComponent<LightComponent>().SetDisplayName("Light Component").SetVisibleInEditor(true);
    setup.RegisterVariable<LightComponent, &LightComponent::lightColor>().SetDisplayName("Color");
    setup.RegisterVariable<LightComponent, &LightComponent::lightIntensity>().SetDisplayName("Intensity");
    setup.RegisterVariable<LightComponent, &LightComponent::lightAttenuation>().SetDisplayName("Attenuation");

    return true;
}
