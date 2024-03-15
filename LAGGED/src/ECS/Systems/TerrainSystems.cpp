#include "TerrainSystems.h"
#include "ECS/Components/TerrainComponents.h"
#include "ECS/Components/BasicComponents.h"

namespace LAG::SurfaceSystems
{
	void GenerateFlatPlane(Entity* entity)
	{
	}

	void GenerateNoiseSurface(Entity* entity)
	{
		entity->GetComponent<ProceduralSurfaceComponent>()->m_Surface.GenerateNoiseSurface(*entity->GetComponent<TransformComponent>(), *entity->GetComponent<ProceduralSurfaceComponent>());
		entity->GetComponent<ProceduralSurfaceComponent>()->m_Surface.Reload();
	}

	void SetHeightMapTexture(Entity* entity, Texture* texture)
	{
	}
}