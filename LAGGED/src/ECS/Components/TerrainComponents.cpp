#include "TerrainComponents.h"

#include "ECS/Scene.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
    bool SurfaceComponent::InitializeReflection()
    {
        SceneReflect& refl = SceneReflect::Get();
        refl.ReflectComponent<SurfaceComponent>().SetDisplayName("Model Component");
        refl.ReflectVariable(&SurfaceComponent::surfaceTexture).SetDisplayName("Texture");

        return true;
    }

    bool ProceduralSurfaceComponent::InitializeReflection()
    {
        SceneReflect& refl = SceneReflect::Get();
        refl.ReflectComponent<ProceduralSurfaceComponent>().SetDisplayName("Procedural Surface Component");

        refl.ReflectVariable(&ProceduralSurfaceComponent::surfaceSubdivisions).SetDisplayName("Subdivision");
        refl.ReflectVariable(&ProceduralSurfaceComponent::noiseProperties).SetDisplayName("Noise properties");
        refl.ReflectVariable(&ProceduralSurfaceComponent::surfaceTexture).SetDisplayName("Texture");

        return true;
    }

}