#include "TerrainComponents.h"

#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionComponentSetup.h"

namespace LAG
{
	bool SurfaceComponent::InitializeReflection()
	{
        Reflection::ReflectionSystem setup;
        setup.RegisterComponent<SurfaceComponent>().SetDisplayName("Surface Component").SetVisibleInEditor(true);
        setup.RegisterVariable<SurfaceComponent, &SurfaceComponent::m_SurfaceTexture>().SetDisplayName("Texture");

        return true;
	}

    bool ProceduralSurfaceComponent::InitializeReflection()
    {
        Reflection::ReflectionSystem setup;
        setup.RegisterComponent<ProceduralSurfaceComponent>().SetDisplayName("Procedural Surface Component").SetVisibleInEditor(true);
        setup.RegisterVariable<ProceduralSurfaceComponent, &ProceduralSurfaceComponent::m_SurfaceSubdivisions>().SetDisplayName("Subdivision");
        setup.RegisterVariable<ProceduralSurfaceComponent, &ProceduralSurfaceComponent::m_NoiseProperties>().SetDisplayName("Noise properties");
        setup.RegisterVariable<ProceduralSurfaceComponent, &ProceduralSurfaceComponent::m_SurfaceTexture>().SetDisplayName("Texture");

        return true;
    }

}