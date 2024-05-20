#include "TerrainComponents.h"

#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionComponentSetup.h"

namespace LAG
{
	bool SurfaceComponent::InitializeReflection()
	{
        Reflection::ReflectionSystem<SurfaceComponent> setup;
        setup.RegisterComponent().SetDisplayName("Surface Component").SetVisibleInEditor(true);
        setup.RegisterVariable<&SurfaceComponent::m_SurfaceTexture>().SetDisplayName("Texture");

        return true;
	}

    bool ProceduralSurfaceComponent::InitializeReflection()
    {
        Reflection::ReflectionSystem<ProceduralSurfaceComponent> setup;
        setup.RegisterComponent().SetDisplayName("Procedural Surface Component").SetVisibleInEditor(true);

        setup.RegisterVariable<&ProceduralSurfaceComponent::m_SurfaceSubdivisions>().SetDisplayName("Subdivision");
        setup.RegisterVariable<&ProceduralSurfaceComponent::m_NoiseProperties>().SetDisplayName("Noise properties");
        setup.RegisterVariable<&ProceduralSurfaceComponent::m_SurfaceTexture>().SetDisplayName("Texture");

        return true;
    }

}