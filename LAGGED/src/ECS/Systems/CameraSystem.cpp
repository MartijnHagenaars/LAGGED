#include "Precomp.h"
#include "CameraSystem.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"

#include "Core/Input/Input.h"

#include "ECS/Components/BasicComponents.h"

namespace LAG::CameraSystem
{
	void Update(uint32_t entityID, TransformComponent& transform, CameraComponent& camera)
	{
		//Only update active cameras
		if (!camera.isActive)
			return;
		
		if (Input::IsActionPressed(Utility::String("cameraMoveForward")))
			transform.position.z += 1.f * camera.movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraMoveBackward")))
			transform.position.z += -1.f * camera.movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraMoveLeft")))
			transform.position.x += 1.f * camera.movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraMoveRight")))
			transform.position.x += -1.f * camera.movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraMoveUp")))
			transform.position.y += 1.f * camera.movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraMoveDown")))
			transform.position.y += -1.f * camera.movementSpeed;

		if (Input::IsActionPressed(Utility::String("cameraLookUp")))
			transform.rotation.x += 1.f * camera.movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraLookDown")))
			transform.rotation.x += -1.f * camera.movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraLookLeft")))
			transform.rotation.y += 1.f * camera.movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraLookRight")))
			transform.rotation.y += -1.f * camera.movementSpeed;


	}
}