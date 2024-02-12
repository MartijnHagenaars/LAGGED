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

		//Add transform component
		float chunkSize = static_cast<float>(CHUNK_SIZE);
		LAG::TransformComponent* transformComp = m_Entity.AddComponent<LAG::TransformComponent>();
		transformComp->SetPosition(glm::vec3(position.x * chunkSize, 0.f, position.y * chunkSize));
		transformComp->SetScale(glm::vec3(chunkSize));

		//Add procedural surface component
		m_Entity.AddComponent<LAG::ProceduralSurfaceComponent>();
		LAG::SurfaceSystems::GenerateNoiseSurface(&m_Entity);
	}
}

void Chunk::Unload()
{
	if (m_Entity.IsValid())
		LAG::GetScene()->RemoveEntity(m_Entity.GetEntityID());
}
