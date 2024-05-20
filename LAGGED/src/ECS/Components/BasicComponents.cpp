#include "BasicComponents.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionComponentSetup.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtc/quaternion.hpp"

bool LAG::DefaultComponent::InitializeReflection()
{
	Reflection::ReflectionSystem<DefaultComponent> setup;
	setup.RegisterComponent().SetDisplayName("Default Component").SetVisibleInEditor(true);
	setup.RegisterVariable<&DefaultComponent::name>().SetDisplayName("Name");
	setup.RegisterVariable<&DefaultComponent::visible>().SetDisplayName("Is Visible");
	return true;
}

LAG::TransformComponent::TransformComponent() :
	m_Translation(glm::vec3(0.f)), m_Rotation(glm::vec3(0.f)), m_Scale(glm::vec3(1.f)), m_Transform(glm::identity<glm::mat4>())
{
}

LAG::TransformComponent::TransformComponent(const glm::vec3& translation) :
	m_Translation(translation), m_Rotation(glm::vec3(0.f)), m_Scale(glm::vec3(1.f)), m_Transform(glm::identity<glm::mat4>())
{}

LAG::TransformComponent::TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) :
	m_Translation(translation), m_Rotation(rotation), m_Scale(scale), m_Transform(glm::identity<glm::mat4>())
{}

void LAG::TransformComponent::SetTransformMatrix(const glm::mat4 transformMat)
{
	m_Dirty = false;
	m_Transform = transformMat;

	glm::quat rotQuat;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_Transform, m_Scale, rotQuat, m_Translation, skew, perspective);
}

const glm::mat4& LAG::TransformComponent::GetTransformMatrix()
{
	if (m_Dirty)
	{
		m_Transform = glm::identity<glm::mat4>();
		m_Transform = glm::translate(m_Transform, m_Translation);
		m_Transform = glm::rotate(m_Transform, m_Rotation.x, glm::vec3(1.f, 0.f, 0.f));
		m_Transform = glm::rotate(m_Transform, m_Rotation.y, glm::vec3(0.f, 1.f, 0.f));
		m_Transform = glm::rotate(m_Transform, m_Rotation.z, glm::vec3(0.f, 0.f, 1.f));
		m_Transform = glm::scale(m_Transform, m_Scale);
		m_Dirty = false;
	}

	return m_Transform;
}

bool LAG::TransformComponent::InitializeReflection()
{
	Reflection::ReflectionSystem<TransformComponent> setup;
	setup.RegisterComponent().SetDisplayName("Transform Component").SetVisibleInEditor(true);

	setup.RegisterVariable<&TransformComponent::m_Translation>().SetDisplayName("Translation");
	setup.RegisterVariable<&TransformComponent::m_Rotation>().SetDisplayName("Rotation");
	setup.RegisterVariable<&TransformComponent::m_Scale>().SetDisplayName("Scale");

	return true;
}
