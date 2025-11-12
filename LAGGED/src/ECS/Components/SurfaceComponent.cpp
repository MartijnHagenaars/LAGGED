#include "SurfaceComponent.h"

#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
    bool SurfaceComponent::InitializeReflection()
    {
        SceneReflect::ReflectComponent<SurfaceComponent>().SetDisplayName("Surface Component");

        SceneReflect::ReflectVariable(&SurfaceComponent::surfaceSubdivisions).SetDisplayName("Subdivision");
        SceneReflect::ReflectVariable(&SurfaceComponent::noiseProperties).SetDisplayName("Noise properties");
        SceneReflect::ReflectVariable(&SurfaceComponent::surfaceTexture).SetDisplayName("Texture");

        return true;
    }

}