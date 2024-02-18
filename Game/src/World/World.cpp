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

		for (int loadDistance = 1; loadDistance < m_LoadDistance; loadDistance++)
		{
			int halfLoadDistance = static_cast<int>(std::ceil(loadDistance * 0.5f));
			for (int z = -halfLoadDistance; z <= halfLoadDistance; z++)
			{
				for (int x = -halfLoadDistance; x <= halfLoadDistance; x++)
				{
					int chunkXPos = -(xCamGridPos + x);
					int chunkZPos = -(zCamGridPos + z);

					ChunkKey ck = ChunkKey(chunkXPos, chunkZPos);
					if (m_ChunkMap.find(ck) == m_ChunkMap.end())
					{
						Chunk newChunk = {};
						newChunk.Load(glm::vec2(chunkXPos, chunkZPos));
						m_ChunkMap.insert({ ck, newChunk });
					}
				}
			}
		}
	}
}
