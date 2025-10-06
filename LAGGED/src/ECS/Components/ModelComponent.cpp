#include "ModelComponent.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
	bool ModelComponent::InitializeReflection()
	{
		SceneReflect::ReflectComponent<ModelComponent>().SetDisplayName("Model Component");

		SceneReflect::ReflectVariable(&ModelComponent::modelHandle).SetDisplayName("Model");

		return true;
	}
}
