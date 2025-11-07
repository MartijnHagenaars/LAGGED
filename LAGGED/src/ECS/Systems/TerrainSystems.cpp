#include "TerrainSystems.h"

#include "Core/Engine.h"
#include "ECS/Components/SurfaceComponent.h"
#include "ECS/Components/BasicComponents.h"

namespace LAG::SurfaceSystems
{
	void GenerateNoiseSurface(EntityID entityID)
	{
		LAG::Scene* sc = LAG::GetScene();
		TransformComponent* transformComp = sc->GetComponent<TransformComponent>(entityID);
		SurfaceComponent* surfaceComp = sc->GetComponent<SurfaceComponent>(entityID);
		surfaceComp->surface.GenerateNoiseSurface(*transformComp, *surfaceComp);

		// TODO: Could use a better method of reloading...
		surfaceComp->surface.Unload();
		surfaceComp->surface.Load();
	}
}