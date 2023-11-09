#include "Precomp.h"
#include "CameraSystem.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/CameraComponent.h"

#include "Platform/Base/Window/WindowManager.h"

#include "Core/Input/Input.h"

#include "ECS/Components/BasicComponents.h"
#include "glm/ext/matrix_transform.hpp"
#include <glm/ext/matrix_clip_space.hpp>

namespace LAG::CameraSystem
{
	void Update(uint32_t entityID)
	{
		CameraComponent* camera = GetScene()->GetEntity(entityID).GetComponent<CameraComponent>();

		//Only update active cameras
		if (!camera->isActive)
			return;

		//float cameraRotationSpeed = 1.f * camera->movementSpeed * GetEngine().

		TransformComponent* transform = GetScene()->GetEntity(entityID).GetComponent<TransformComponent>();
		glm::vec3 preCameraPosition = transform->position;
		if (Input::IsActionPressed(Utility::String("cameraMoveForward")))
			transform->position.z += 1.f * camera->movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraMoveBackward")))
			transform->position.z += -1.f * camera->movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraMoveLeft")))
			transform->position.x += 1.f * camera->movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraMoveRight")))
			transform->position.x += -1.f * camera->movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraMoveUp")))
			transform->position.y += 1.f * camera->movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraMoveDown")))
			transform->position.y += -1.f * camera->movementSpeed;

		if (preCameraPosition != transform->position)
			camera->hasCameraMoved = true;

		glm::vec3 preCameraRotation = transform->rotation;
		if (Input::IsActionPressed(Utility::String("cameraLookUp")))
			transform->rotation.x += 1.f * camera->movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraLookDown")))
			transform->rotation.x += -1.f * camera->movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraLookLeft")))
			transform->rotation.y += 1.f * camera->movementSpeed;
		if (Input::IsActionPressed(Utility::String("cameraLookRight")))
			transform->rotation.y += -1.f * camera->movementSpeed;

		if (preCameraRotation != transform->rotation)
			camera->hasCameraMoved = true;
		
	}
	
	glm::mat4 CalculateViewMat(uint32_t entityID)
	{
		CameraComponent* camera = GetScene()->GetEntity(entityID).GetComponent<CameraComponent>();

		if (!camera->hasCameraMoved)
			return camera->viewMat;

		TransformComponent* transform = GetScene()->GetEntity(entityID).GetComponent<TransformComponent>();

		camera->viewMat = glm::mat4(1.f);
		camera->viewMat = glm::translate(camera->viewMat, transform->position);
		camera->viewMat = glm::rotate(camera->viewMat, transform->rotation.x, glm::vec3(1.f, 0.f, 0.f));
		camera->viewMat = glm::rotate(camera->viewMat, transform->rotation.y, glm::vec3(0.f, 1.f, 0.f));
		camera->viewMat = glm::rotate(camera->viewMat, transform->rotation.z, glm::vec3(0.f, 0.f, 1.f));

		camera->hasCameraMoved = false;
		return camera->viewMat;
	}

	glm::mat4 CalculateProjMat(uint32_t entityID)
	{
		CameraComponent* camera = GetScene()->GetEntity(entityID).GetComponent<CameraComponent>();
		if (!camera->hasCameraDimensionChanged)
			return camera->projMat;

		camera->projMat = glm::mat4(1.f);
		camera->projMat = glm::perspective(glm::radians(camera->fov), static_cast<float>(GetWindowManager()->GetFocussedWindow()->GetWidth()) / GetWindowManager()->GetFocussedWindow()->GetHeight(), 0.1f, 100.f);
		camera->hasCameraDimensionChanged = false;
		return camera->projMat;
	}
}