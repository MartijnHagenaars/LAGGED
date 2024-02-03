#include "BasicComponents.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionDefines.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtc/quaternion.hpp"

bool LAG::DefaultComponent::InitializeReflection()
{
    auto factory = entt::meta<DefaultComponent>();
    factory.type(entt::type_hash<DefaultComponent>::value());

    factory.data<&DefaultComponent::name>(entt::hashed_string("name")).prop(Reflection::DISPLAY_NAME, std::string("Name"));

    return false; 
}

LAG::TransformComponent::TransformComponent() : 
    translation(glm::vec3(0.f)), rotation(glm::vec3(0.f)), scale(glm::vec3(1.f)), transform(glm::identity<glm::mat4>())
{
}

void LAG::TransformComponent::SetTransformMatrix(const glm::mat4 transformMat)
{
    dirty = false;
    transform = transformMat; 

    glm::quat rotQuat;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transform, scale, rotQuat, translation, skew, perspective);
}

const glm::mat4& LAG::TransformComponent::GetTransformMatrix()
{
    if (dirty)
    {
        transform = glm::identity<glm::mat4>();
        transform = glm::translate(transform, translation);
        transform = glm::rotate(transform, rotation.x, glm::vec3(1.f, 0.f, 0.f));
        transform = glm::rotate(transform, rotation.y, glm::vec3(0.f, 1.f, 0.f));
        transform = glm::rotate(transform, rotation.z, glm::vec3(0.f, 0.f, 1.f));
        transform = glm::scale(transform, scale);
        dirty = false;
    }

    return transform;
}

bool LAG::TransformComponent::InitializeReflection()
{
    auto factory = entt::meta<TransformComponent>();
    factory.type(entt::type_hash<TransformComponent>::value());

    factory.data<&TransformComponent::translation>(entt::hashed_string("translation")).prop(Reflection::DISPLAY_NAME, std::string("Position"));
    factory.data<&TransformComponent::rotation>(entt::hashed_string("rotation")).prop(Reflection::DISPLAY_NAME, std::string("Rotation"));
    factory.data<&TransformComponent::scale>(entt::hashed_string("scale")).prop(Reflection::DISPLAY_NAME, std::string("Scale"));

    return false;
}