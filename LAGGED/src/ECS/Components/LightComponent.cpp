#include "LightComponent.h"

#include "ECS/Scene.h"

bool LAG::LightComponent::InitializeReflection()
{
    //Reflection::ReflectionSystem<LightComponent> setup;
    //setup.RegisterComponent().SetDisplayName("Light Component").SetVisibleInEditor(true);
    //
    //setup.RegisterVariable<&LightComponent::lightColor>().SetDisplayName("Color");
    //setup.RegisterVariable<&LightComponent::lightIntensity>().SetDisplayName("Intensity");
    //setup.RegisterVariable<&LightComponent::lightAttenuation>().SetDisplayName("Attenuation");

    return true;
}
