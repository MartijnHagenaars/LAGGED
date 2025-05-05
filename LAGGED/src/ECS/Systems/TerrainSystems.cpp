#include "TerrainSystems.h"
#include "ECS/Components/TerrainComponents.h"
#include "ECS/Components/BasicComponents.h"

namespace LAG::SurfaceSystems
{
	void GenerateFlatPlane(Entity* entity)
	{
	}

	void GenerateNoiseSurface(Entity& entity)
	{
		ProceduralSurfaceComponent* surfaceComp = entity.GetComponent<ProceduralSurfaceComponent>();
		surfaceComp->surface.GenerateNoiseSurface(*entity.GetComponent<TransformComponent>(), *entity.GetComponent<ProceduralSurfaceComponent>());

		//TODO: Could use a better method of reloading...
		surfaceComp->surface.Unload();
		surfaceComp->surface.Load();
	}

	void SetHeightMapTexture(Entity* entity, Texture* texture)
	{
	}
}