#include "Precomp.h"
#include "BasicSystems.h"
#include "Core/Engine.h"
#include "ECS/Scene.h"

#include "ECS/Components/BasicComponents.h"

namespace LAG
{
	void BasicSystems::UpdateBasicSystems()
	{
		GetScene()->Loop<SinWaveComponent>([](uint32_t entityID, SinWaveComponent& component)
			{
				SinWaveSystem::Update(entityID);
			});
	}

	void SinWaveSystem::Update(uint32_t entityID)
	{
		SinWaveComponent* component = GetScene()->GetEntity(entityID).GetComponent<SinWaveComponent>();
		component->time += GetEngine().GetDeltaTime();
		component->sinValue = component->amplitude * sinf(component->frequency * component->time);
	}
}