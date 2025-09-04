#include "TerrainComponents.h"

#include "ECS/Scene.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
    bool SurfaceComponent::InitializeReflection()
    {
        SceneReflect& refl = SceneReflect::Get();
        refl.Register<SurfaceComponent>().SetDisplayName("Model Component");
        refl.RegisterVariable(&SurfaceComponent::surfaceTexture).SetDisplayName("Texture");

        return true;
    }

    bool ProceduralSurfaceComponent::InitializeReflection()
    {
        SceneReflect& refl = SceneReflect::Get();
        refl.Register<ProceduralSurfaceComponent>().SetDisplayName("Procedural Surface Component");

        refl.RegisterVariable(&ProceduralSurfaceComponent::surfaceSubdivisions).SetDisplayName("Subdivision");
        refl.RegisterVariable(&ProceduralSurfaceComponent::noiseProperties).SetDisplayName("Noise properties");
        refl.RegisterVariable(&ProceduralSurfaceComponent::surfaceTexture).SetDisplayName("Texture");

        return true;
    }

}