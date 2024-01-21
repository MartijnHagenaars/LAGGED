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

		float cameraMovementSpeed = 1.f * camera->movementSpeed * GetEngine().GetDeltaTime();
		float cameraRotationSpeed = 1.f * camera->rotationSpeed * GetEngine().GetDeltaTime();

		TransformComponent* transform = GetScene()->GetEntity(entityID).GetComponent<TransformComponent>();
		glm::vec3 preCameraPosition = transform->position;
		if (Input::IsActionPressed(HashedString("cameraMoveForward")))
			transform->position += camera->forwardVector * cameraMovementSpeed;
		if (Input::IsActionPressed(HashedString("cameraMoveBackward")))
			transform->position -= camera->forwardVector * cameraMovementSpeed;
		if (Input::IsActionPressed(HashedString("cameraMoveLeft")))
			transform->position += camera->rightVector * cameraMovementSpeed;
		if (Input::IsActionPressed(HashedString("cameraMoveRight")))
			transform->position -= camera->rightVector * cameraMovementSpeed;
		if (Input::IsActionPressed(HashedString("cameraMoveUp")))
			transform->position.y -= cameraMovementSpeed;
		if (Input::IsActionPressed(HashedString("cameraMoveDown")))
			transform->position.y += cameraMovementSpeed;

		if (preCameraPosition != transform->position)
			camera->hasCameraMoved = true;

		glm::vec3 preCameraRotation = transform->rotation;
		if (Input::IsActionPressed(HashedString("cameraLookUp")))
			transform->rotation.x -= cameraRotationSpeed;
		if (Input::IsActionPressed(HashedString("cameraLookDown")))
			transform->rotation.x += cameraRotationSpeed;
		if (Input::IsActionPressed(HashedString("cameraLookLeft")))
			transform->rotation.y -= cameraRotationSpeed;
		if (Input::IsActionPressed(HashedString("cameraLookRight")))
			transform->rotation.y += cameraRotationSpeed;

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
		camera->viewMat = glm::rotate(camera->viewMat, transform->rotation.x, glm::vec3(1.f, 0.f, 0.f));
		camera->viewMat = glm::rotate(camera->viewMat, transform->rotation.y, glm::vec3(0.f, 1.f, 0.f));
		camera->viewMat = glm::rotate(camera->viewMat, transform->rotation.z, glm::vec3(0.f, 0.f, 1.f));
		camera->viewMat = glm::translate(camera->viewMat, transform->position);

		//Thanks to this post for helping fix forward- and right vector calculations
		//  https://stackoverflow.com/questions/50081475/opengl-local-up-and-right-from-matrix-4x4
		camera->forwardVector = glm::vec3(camera->viewMat[0][2], camera->viewMat[1][2], camera->viewMat[2][2]);
		camera->rightVector = glm::vec3(camera->viewMat[0][0], camera->viewMat[1][0], camera->viewMat[2][0]);

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