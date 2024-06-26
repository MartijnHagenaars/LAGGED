#include "BasicSystems.h"
#include "Core/Engine.h"
#include "ECS/Scene.h"

#include "ECS/Components/BasicComponents.h"

namespace LAG
{
	void BasicSystems::UpdateBasicSystems()
	{
		GetScene()->Loop<SinWaveComponent>([](Entity entity, SinWaveComponent& component)
			{
				SinWaveSystem::Update(&entity);
			});
	}

	void SinWaveSystem::Update(Entity* entity)
	{
		SinWaveComponent* component = entity->GetComponent<SinWaveComponent>();
		component->time += GetEngine().GetDeltaTime();
		component->sinValue = component->amplitude * sinf(component->frequency * component->time);
	}
}