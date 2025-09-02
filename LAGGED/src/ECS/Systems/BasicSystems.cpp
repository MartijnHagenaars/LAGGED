#include "BasicSystems.h"
#include "Core/Engine.h"
#include "ECS/Scene.h"

#include "ECS/Components/BasicComponents.h"

namespace LAG
{
	void BasicSystems::UpdateBasicSystems()
	{
		GetScene()->ForEach<SinWaveComponent>([](EntityID entity, SinWaveComponent* component)
			{
				component->time += GetEngine().GetDeltaTime();
				component->sinValue = component->amplitude * sinf(component->frequency * component->time);
			});
	}
}