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
		entity.GetComponent<ProceduralSurfaceComponent>()->surface.GenerateNoiseSurface(*entity.GetComponent<TransformComponent>(), *entity.GetComponent<ProceduralSurfaceComponent>());
		entity.GetComponent<ProceduralSurfaceComponent>()->surface.Reload();
	}

	void SetHeightMapTexture(Entity* entity, Texture* texture)
	{
	}
}