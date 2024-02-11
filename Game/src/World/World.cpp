#include "World.h"

#include "Core/Engine.h"
#include "ECS/Entity.h"
#include "ECS/Scene.h"

#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/TerrainComponents.h"
#include "ECS/Components/CameraComponent.h"
#include "Chunk.h"

World::World(int loadDistance) : 
	m_LoadDistance(loadDistance)
{
}

World::~World()
{
}

void World::Update()
{
	LAG::Entity cameraEntity = {};
	LAG::GetEngine().GetScene()->Loop<LAG::CameraComponent>([&cameraEntity](LAG::Entity entity, LAG::CameraComponent& cameraComp)
		{
			if(cameraComp.isActive)
				cameraEntity = entity;
		});

	if (!cameraEntity.IsValid())
	{
		LAG::Logger::Error("Failed to find camera");
		return;
	}

	LAG::TransformComponent* camTransform = cameraEntity.GetComponent<LAG::TransformComponent>();
	if (camTransform->GetPosition() != m_PrevCameraPos)
	{
		m_PrevCameraPos = camTransform->GetPosition();

		int xCamGridPos = static_cast<int>(camTransform->GetPosition().x / static_cast<float>(CHUNK_SIZE));
		int zCamGridPos = static_cast<int>(camTransform->GetPosition().z / static_cast<float>(CHUNK_SIZE));
		printf("Camera position in chunk world: %i %i\n", xCamGridPos, zCamGridPos);
		
		for (int h = -(m_LoadDistance * .5f); h <= (m_LoadDistance * .5f); h++)
		{
			for (int w = -(m_LoadDistance * .5f); w <= (m_LoadDistance * .5f); w++)
			{
				if (m_ChunkMap.find(ChunkKey(w, h)) == m_ChunkMap.end())
				{
					m_ChunkMap.insert({ ChunkKey(w, h), Chunk(glm::vec2(w, h)) });
				}

				printf("%i %i - ", w, h);
			}
			printf("\n");
		}
		printf("======================\n\n");
	}
}
