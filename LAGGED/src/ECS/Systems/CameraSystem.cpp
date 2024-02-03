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

#include "ImGui/imgui.h"

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
		glm::vec3 camPosAdjustment = glm::vec3(0.f);
		if (Input::IsActionPressed(HashedString("cameraMoveForward")))
			camPosAdjustment += camera->forwardVector * cameraMovementSpeed;
		if (Input::IsActionPressed(HashedString("cameraMoveBackward")))
			camPosAdjustment -= camera->forwardVector * cameraMovementSpeed;
		if (Input::IsActionPressed(HashedString("cameraMoveLeft")))
			camPosAdjustment += camera->rightVector * cameraMovementSpeed;
		if (Input::IsActionPressed(HashedString("cameraMoveRight")))
			camPosAdjustment -= camera->rightVector * cameraMovementSpeed;
		if (Input::IsActionPressed(HashedString("cameraMoveUp")))
			camPosAdjustment.y -= cameraMovementSpeed;
		if (Input::IsActionPressed(HashedString("cameraMoveDown")))
			camPosAdjustment.y += cameraMovementSpeed;

		if (camPosAdjustment != glm::vec3(0.f))
		{
			transform->SetPosition(transform->GetPosition() + camPosAdjustment);
			camera->hasCameraMoved = true;
		}

		glm::vec3 camRotAdjustment = glm::vec3(0.f);
		if (Input::IsActionPressed(HashedString("cameraLookUp")))
			camRotAdjustment.x -= cameraRotationSpeed;
		if (Input::IsActionPressed(HashedString("cameraLookDown")))
			camRotAdjustment.x += cameraRotationSpeed;
		if (Input::IsActionPressed(HashedString("cameraLookLeft")))
			camRotAdjustment.y -= cameraRotationSpeed;
		if (Input::IsActionPressed(HashedString("cameraLookRight")))
			camRotAdjustment.y += cameraRotationSpeed;

		if (camRotAdjustment != glm::vec3(0.f))
		{
			transform->SetRotation(transform->GetRotation() + camRotAdjustment);
			camera->hasCameraMoved = true;
		}
		
	}
	
	glm::mat4 CalculateViewMat(uint32_t entityID)
	{
		CameraComponent* camera = GetScene()->GetEntity(entityID).GetComponent<CameraComponent>();
		TransformComponent* transform = GetScene()->GetEntity(entityID).GetComponent<TransformComponent>();
		if (camera == nullptr || transform == nullptr)
			return glm::mat4(0.f);

		if (camera->hasCameraMoved)
		{
			camera->viewMat = glm::mat4(1.f);
			camera->viewMat = glm::rotate(camera->viewMat, transform->GetRotation().x, glm::vec3(1.f, 0.f, 0.f));
			camera->viewMat = glm::rotate(camera->viewMat, transform->GetRotation().y, glm::vec3(0.f, 1.f, 0.f));
			camera->viewMat = glm::rotate(camera->viewMat, transform->GetRotation().z, glm::vec3(0.f, 0.f, 1.f));
			camera->viewMat = glm::translate(camera->viewMat, transform->GetPosition());

			//Thanks to this post for helping fix forward- and right vector calculations
			//  https://stackoverflow.com/questions/50081475/opengl-local-up-and-right-from-matrix-4x4
			camera->forwardVector = glm::vec3(transform->GetTransformMatrix()[0][2], transform->GetTransformMatrix()[1][2], transform->GetTransformMatrix()[2][2]);
			camera->rightVector = glm::vec3(transform->GetTransformMatrix()[0][0], transform->GetTransformMatrix()[1][0], transform->GetTransformMatrix()[2][0]);

			camera->hasCameraMoved = false;
		}

		return camera->viewMat;
	}

	glm::mat4 CalculateProjMat(uint32_t entityID)
	{
		CameraComponent* camera = GetScene()->GetEntity(entityID).GetComponent<CameraComponent>();
		if (!camera->hasCameraDimensionChanged)
			return camera->projMat;

		camera->projMat = glm::mat4(1.f);
		camera->projMat = glm::perspective(glm::radians(camera->fov), static_cast<float>(GetWindowManager()->GetFocussedWindow()->GetWidth()) / GetWindowManager()->GetFocussedWindow()->GetHeight(), 0.1f, 100000.f);
		camera->hasCameraDimensionChanged = false;
		return camera->projMat;
	}
}