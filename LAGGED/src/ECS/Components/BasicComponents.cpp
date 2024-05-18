#include "BasicComponents.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionComponentSetup.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtc/quaternion.hpp"

bool LAG::DefaultComponent::InitializeReflection()
{
	Reflection::ReflectionSystem setup;
	setup.RegisterComponent<DefaultComponent>().SetDisplayName("Default Component").SetVisibleInEditor(false);


	//setup.RegisterVariable<&DefaultComponent::name>();

	//setup.RegisterVariable<decltype(&DefaultComponent::name)>().SetDisplayName("Name");
	//setup.RegisterVariable<decltype(name)>().SetDisplayName("Name");


	//TODO: A thing needs to be added for automatially assigning

	//.ReflectVariable(VARIABLE_REFLECT(DefaultComponent::name)).SetDisplayName("Name").SetCallback(...);
	
	//auto factory = entt::meta<DefaultComponent>();
	//factory.type(entt::type_hash<DefaultComponent>::value());
	//factory.data<&DefaultComponent::name>(entt::hashed_string("name")).prop(Reflection::VariableProperties::DISPLAY_NAME, std::string("Name"));

	return false;
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
	auto factory = entt::meta<TransformComponent>();
	factory.type(entt::type_hash<TransformComponent>::value()).prop(Reflection::VariableProperties::DISPLAY_NAME, std::string("Transform Component"));;

	factory.data<&TransformComponent::m_Translation>(entt::hashed_string("translation")).prop(Reflection::VariableProperties::DISPLAY_NAME, std::string("Position"));
	factory.data<&TransformComponent::m_Rotation>(entt::hashed_string("rotation")).prop(Reflection::VariableProperties::DISPLAY_NAME, std::string("Rotation"));
	factory.data<&TransformComponent::m_Scale>(entt::hashed_string("scale")).prop(Reflection::VariableProperties::DISPLAY_NAME, std::string("Scale"));

	return false;
}
