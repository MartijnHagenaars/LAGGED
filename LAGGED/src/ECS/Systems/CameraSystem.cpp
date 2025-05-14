#include "CameraSystem.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <ImGui/imgui.h>

#include "Core/Engine.h"
#include "Core/Input/Input.h"
#include "Platform/Window.h"

#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/BasicComponents.h"

namespace LAG::CameraSystem
{
	void Update(EntityID entityID)
	{
		if (entityID == ENTITY_NULL)
		{
			CRITICAL("Camera Entity ID is nullptr.");
			return;
		}

		CameraComponent* camera = GetEngine().GetScene()->GetComponent<CameraComponent>(entityID);
		if (!camera)
			LAG_ASSERT("Camera Entity ID does not contain CameraComponent.");

		//Only update active cameras
		if (!camera->isActive)
			return;

		float cameraMovementSpeed = 1.f * camera->movementSpeed * GetEngine().GetDeltaTime();
		float cameraRotationSpeed = 1.f * camera->rotationSpeed * GetEngine().GetDeltaTime();

		TransformComponent* transform = GetEngine().GetScene()->GetComponent<TransformComponent>(entityID);
		if (!transform)
			CRITICAL("Camera Entity ID does not contain TransformComponent.");

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

	EntityID GetActiveCameraEntityID()
	{
		std::vector<EntityID> camEntities = GetScene()->GetEntitiesWithComponents<CameraComponent>();
		if (camEntities.empty())
		{
			WARNING("Failed to find active camera in GetActiveCameraEntity()");
			return ENTITY_NULL;
		}

		for (const auto& id : camEntities)
		{
			CameraComponent* camComp = GetScene()->GetComponent<CameraComponent>(id);
			if (camComp != nullptr && camComp->isActive)
				return id;
		}

		return ENTITY_NULL;
	}

	glm::mat4 CalculateViewMat(EntityID entityID)
	{
		CameraComponent* camera = GetScene()->GetComponent<CameraComponent>(entityID);
		TransformComponent* transform = GetScene()->GetComponent<TransformComponent>(entityID);
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

	glm::mat4 CalculateProjMat(EntityID entityID)
	{
		CameraComponent* camera = GetScene()->GetComponent<CameraComponent>(entityID);
		if (!camera->hasCameraDimensionChanged)
			return camera->projMat;

		camera->projMat = glm::mat4(1.f);
		float aspectRatio = static_cast<float>(camera->frameBuffer->GetSize().x) / camera->frameBuffer->GetSize().y;
		camera->projMat = glm::perspective(glm::radians(camera->fov), aspectRatio, 0.1f, 100000.f);
		camera->hasCameraDimensionChanged = false;
		return camera->projMat;
	}
}