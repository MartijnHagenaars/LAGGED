#include "TerrainSystems.h"

#include "Core/Engine.h"
#include "ECS/Components/TerrainComponents.h"
#include "ECS/Components/BasicComponents.h"

namespace LAG::SurfaceSystems
{
	void GenerateNoiseSurface(EntityID entityID)
	{
		LAG::Scene* sc = LAG::GetScene();
		TransformComponent* transformComp = sc->GetComponent<TransformComponent>(entityID);
		ProceduralSurfaceComponent* surfaceComp = sc->GetComponent<ProceduralSurfaceComponent>(entityID);
		surfaceComp->surface.GenerateNoiseSurface(*transformComp, *surfaceComp);

		// TODO: Could use a better method of reloading...
		surfaceComp->surface.Unload();
		surfaceComp->surface.Load();
	}
}