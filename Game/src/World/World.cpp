#include "World.h"

#include "Core/Engine.h"
#include "ECS/Entity.h"
#include "ECS/Scene.h"

#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/TerrainComponents.h"
#include "ECS/Components/CameraComponent.h"
#include "Chunk.h"

World::World(int loadDistance, bool infiniteTerrain) :
	m_LoadDistance(loadDistance), m_UseInfiniteTerrain(infiniteTerrain)
{
}

World::~World()
{
}

void World::Update()
{
	if (!m_UseInfiniteTerrain && m_HasGeneratedTerrain)
		return;

	LAG::EntityID camEntityID = LAG::ENTITY_NULL;
	LAG::GetEngine().GetScene()->RunSystem<LAG::CameraComponent>([&camEntityID](LAG::EntityID entity, LAG::CameraComponent* cameraComp)
		{
			if(cameraComp->isActive)
				camEntityID = entity;
		});

	if (camEntityID == LAG::ENTITY_NULL)
	{
		ERROR("Failed to find camera");
		return;
	}


	LAG::TransformComponent* camTransform = LAG::GetEngine().GetScene()->GetComponent<LAG::TransformComponent>(camEntityID);
	if (camTransform->GetPosition() != m_PrevCameraPos)
	{
		m_PrevCameraPos = camTransform->GetPosition();
		m_UpdateChunks = true;
	}

	if(m_UpdateChunks)
	{
		int xCamGridPos = static_cast<int>(camTransform->GetPosition().x / static_cast<float>(CHUNK_SIZE));
		int zCamGridPos = static_cast<int>(camTransform->GetPosition().z / static_cast<float>(CHUNK_SIZE));

		//Add new chunks if necessary
		for (int loadDistance = 1; loadDistance < m_LoadDistance; loadDistance++)
		{
			int halfLoadDistance = static_cast<int>(std::floor(loadDistance * 0.5f));
			for (int z = -halfLoadDistance + 1; z <= halfLoadDistance; z++)
			{
				for (int x = -halfLoadDistance + 1; x <= halfLoadDistance; x++)
				{
					int chunkXPos = -(xCamGridPos + x);
					int chunkZPos = -(zCamGridPos + z);

					ChunkKey ck = ChunkKey(chunkXPos, chunkZPos);
					if (m_ChunkMap.find(ck) == m_ChunkMap.end())
					{
						Chunk newChunk = {};
						newChunk.Load(glm::vec2(chunkXPos, chunkZPos));
						m_ChunkMap.insert({ ck, newChunk });
						return;
					}
				}
			}
		}

		//Unloading of distant chunks
		for (auto it = m_ChunkMap.begin(); it != m_ChunkMap.end();)
		{
			glm::vec2 chunkPos = glm::vec2(it->first.m_X, it->first.m_Y);
			glm::vec2 camPos = glm::vec2(-xCamGridPos, -zCamGridPos);
			float length = glm::length(chunkPos - camPos);
			if (static_cast<int>(length) > static_cast<int>(m_LoadDistance * 0.5f))
			{
				it->second.Unload();
				m_ChunkMap.erase(it++);
			}
			else ++it;
		}

		m_HasGeneratedTerrain = true;
	}
}
