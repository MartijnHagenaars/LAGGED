#include "ModelComponent.h"
#include "ECS/Meta/ReflectionComponentSetup.h"

namespace LAG
{
	bool ModelComponent::InitializeReflection()
	{
		Reflection::ReflectionSystem<ModelComponent> setup;
		setup.RegisterComponent().SetDisplayName("Model Component").SetVisibleInEditor(true);
		setup.RegisterVariable<&ModelComponent::modelHandle>().SetDisplayName("Model");

		return true;
	}
}
