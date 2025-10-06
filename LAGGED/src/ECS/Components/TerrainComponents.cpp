#include "TerrainComponents.h"

#include "ECS/Scene.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
    bool SurfaceComponent::InitializeReflection()
    {
        SceneReflect::ReflectComponent<SurfaceComponent>().SetDisplayName("Model Component");
        SceneReflect::ReflectVariable(&SurfaceComponent::surfaceTexture).SetDisplayName("Texture");

        return true;
    }

    bool ProceduralSurfaceComponent::InitializeReflection()
    {
        SceneReflect::ReflectComponent<ProceduralSurfaceComponent>().SetDisplayName("Procedural Surface Component");

        SceneReflect::ReflectVariable(&ProceduralSurfaceComponent::surfaceSubdivisions).SetDisplayName("Subdivision");
        SceneReflect::ReflectVariable(&ProceduralSurfaceComponent::noiseProperties).SetDisplayName("Noise properties");
        SceneReflect::ReflectVariable(&ProceduralSurfaceComponent::surfaceTexture).SetDisplayName("Texture");

        return true;
    }

}