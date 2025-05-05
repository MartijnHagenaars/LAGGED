#include "TerrainComponents.h"

#include "ECS/Scene.h"

namespace LAG
{
	bool SurfaceComponent::InitializeReflection()
	{
        //Reflection::ReflectionSystem<SurfaceComponent> setup;
        //setup.RegisterComponent().SetDisplayName("Surface Component").SetVisibleInEditor(true);
        //setup.RegisterVariable<&SurfaceComponent::surfaceTexture>().SetDisplayName("Texture");

        return true;
	}

    bool ProceduralSurfaceComponent::InitializeReflection()
    {
        //Reflection::ReflectionSystem<ProceduralSurfaceComponent> setup;
        //setup.RegisterComponent().SetDisplayName("Procedural Surface Component").SetVisibleInEditor(true);

        //setup.RegisterVariable<&ProceduralSurfaceComponent::surfaceSubdivisions>().SetDisplayName("Subdivision");
        //setup.RegisterVariable<&ProceduralSurfaceComponent::noiseProperties>().SetDisplayName("Noise properties");
        //setup.RegisterVariable<&ProceduralSurfaceComponent::surfaceTexture>().SetDisplayName("Texture");

        return true;
    }

}