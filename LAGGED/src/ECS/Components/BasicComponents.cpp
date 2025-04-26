#include "BasicComponents.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
	bool DefaultComponent::InitializeReflection()
	{
		SceneReflect refl;
		refl.RegisterComponent<DefaultComponent>().SetDisplayName("General data");

		refl.RegisterVariable(&DefaultComponent::name).SetDisplayName("Name");
		refl.RegisterVariable(&DefaultComponent::visible).SetDisplayName("Is Visible");

		return true;
	}

	TransformComponent::TransformComponent() :
		translation(glm::vec3(0.f)), rotation(glm::vec3(0.f)), scale(glm::vec3(1.f)), transform(glm::identity<glm::mat4>())
	{
	}

	TransformComponent::TransformComponent(const glm::vec3& translation) :
		translation(translation), rotation(glm::vec3(0.f)), scale(glm::vec3(1.f)), transform(glm::identity<glm::mat4>())
	{}

	TransformComponent::TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) :
		translation(translation), rotation(rotation), scale(scale), transform(glm::identity<glm::mat4>())
	{}

	void TransformComponent::SetTransformMatrix(const glm::mat4 transformMat)
	{
		m_Dirty = false;
		transform = transformMat;

		glm::quat rotQuat;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transform, scale, rotQuat, translation, skew, perspective);
	}

	const glm::mat4& TransformComponent::GetTransformMatrix()
	{
		if (m_Dirty)
		{
			transform = glm::identity<glm::mat4>();
			transform = glm::translate(transform, translation);
			transform = glm::rotate(transform, rotation.x, glm::vec3(1.f, 0.f, 0.f));
			transform = glm::rotate(transform, rotation.y, glm::vec3(0.f, 1.f, 0.f));
			transform = glm::rotate(transform, rotation.z, glm::vec3(0.f, 0.f, 1.f));
			transform = glm::scale(transform, scale);
			m_Dirty = false;
		}

		return transform;
	}

	bool TransformComponent::InitializeReflection()
	{
		SceneReflect refl;
		refl.RegisterComponent<TransformComponent>().SetDisplayName("Transform Component");

		refl.RegisterVariable(&TransformComponent::translation).SetDisplayName("Translation");
		refl.RegisterVariable(&TransformComponent::rotation).SetDisplayName("Rotation");
		refl.RegisterVariable(&TransformComponent::scale).SetDisplayName("Scale");

		refl.RegisterVariable(&TransformComponent::m_Dirty).SetHidden(true).SetReadOnly(true);

		return true;
	}

}