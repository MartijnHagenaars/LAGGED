#include "Chunk.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"

#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/TerrainComponents.h"
#include "ECS/Systems/TerrainSystems.h"

void Chunk::Load(const glm::vec2& position)
{
	LAG::Scene* sc = LAG::GetScene();
	if (!sc->IsValid(m_EntityID))
	{
		std::string entityName = "Terrain (" + std::to_string(static_cast<int>(position.x)) + ", " + std::to_string(static_cast<int>(position.y)) + ")";
		m_EntityID = sc->AddEntity(entityName);

		//TODO: The 64 multiplications and surfaceSubdivisions should be adjustable. Terrain gen currently breaks when surfaceSubdivisions are not equal to the position/scale multiplication

		//Add transform component
		LAG::TransformComponent* transformComp = sc->AddComponent<LAG::TransformComponent>(m_EntityID);
		transformComp->SetPosition(glm::vec3(position.x * 64, 0.f, position.y * 64));
		transformComp->SetScale(glm::vec3(64));

		//Add procedural surface component
		LAG::ProceduralSurfaceComponent* procSurface = sc->AddComponent<LAG::ProceduralSurfaceComponent>(m_EntityID);
		procSurface->surfaceSubdivisions = 64;
		procSurface->noiseProperties.m_Amplitude = 0.7f;
		procSurface->noiseProperties.m_Frequency = 0.5f;
		LAG::SurfaceSystems::GenerateNoiseSurface(m_EntityID);
	}
	//TODO: Noise movement is incorrecct
}

void Chunk::Unload()
{
	LAG::Scene* sc = LAG::GetScene();
	if (sc->IsValid(m_EntityID))
	{
		//TODO: The user should not have to do this. There should be some sort of function that gets all components and checks if they have unload functions.
		//		If they do, the Unload function is called.
		sc->GetComponent<LAG::ProceduralSurfaceComponent>(m_EntityID)->surface.Unload();
		sc->RemoveEntity(m_EntityID);
	}
}
