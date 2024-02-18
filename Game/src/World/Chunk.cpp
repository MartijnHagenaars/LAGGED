#include "Chunk.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"

#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/TerrainComponents.h"
#include "ECS/Systems/TerrainSystems.h"

void Chunk::Load(const glm::vec2& position)
{
	if (!m_Entity.IsValid())
	{
		m_Entity = LAG::GetScene()->AddEntity();

		//TODO: The 64 multiplications and surfaceSubdivisions should be adjustable. Terrain gen currently breaks when surfaceSubdivisions are not equal to the position/scale multiplication

		//Add transform component
		LAG::TransformComponent* transformComp = m_Entity.AddComponent<LAG::TransformComponent>();
		transformComp->SetPosition(glm::vec3(position.x * 64, 0.f, position.y * 64));
		transformComp->SetScale(glm::vec3(64));

		//Add procedural surface component
		m_Entity.AddComponent<LAG::ProceduralSurfaceComponent>()->m_SurfaceSubdivisions = 64;
		LAG::SurfaceSystems::GenerateNoiseSurface(&m_Entity);
	}
	//TODO: Noise movement is incorrecct
}

void Chunk::Unload()
{
	if (m_Entity.IsValid())
		LAG::GetScene()->RemoveEntity(m_Entity.GetEntityID());
}
