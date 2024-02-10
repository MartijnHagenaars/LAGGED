#include "TerrainComponents.h"

#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionDefines.h"

namespace LAG
{
    SurfaceComponent::SurfaceComponent()
    {

    }

    SurfaceComponent::~SurfaceComponent()
    {

    }

	bool SurfaceComponent::InitializeReflection()
	{
        auto factory = entt::meta<SurfaceComponent>();
        factory.type(entt::type_hash<SurfaceComponent>::value());

        //factory.data<&SurfaceComponent::m_Surface>(entt::hashed_string("m_Surface")).prop(Reflection::DISPLAY_NAME, std::string("Surface"));
        factory.data<&SurfaceComponent::m_SurfaceTexture>(entt::hashed_string("m_SurfaceTexture")).prop(Reflection::DISPLAY_NAME, std::string("Texture"));

        return false;
	}


    ProceduralSurfaceComponent::ProceduralSurfaceComponent()
    {
    }

    ProceduralSurfaceComponent::~ProceduralSurfaceComponent()
    {
    }

    bool ProceduralSurfaceComponent::InitializeReflection()
    {
        auto factory = entt::meta<ProceduralSurfaceComponent>();
        factory.type(entt::type_hash<ProceduralSurfaceComponent>::value());

        //factory.data<&ProceduralSurfaceComponent::m_Surface>(entt::hashed_string("m_Surface")).prop(Reflection::DISPLAY_NAME, std::string("Surface"));
        factory.data<&ProceduralSurfaceComponent::m_SurfaceTexture>(entt::hashed_string("m_SurfaceTexture")).prop(Reflection::DISPLAY_NAME, std::string("Texture"));
        factory.data<&ProceduralSurfaceComponent::m_NoiseData>(entt::hashed_string("m_NoiseData")).prop(Reflection::DISPLAY_NAME, std::string("Noise Data"));
        factory.data<&ProceduralSurfaceComponent::m_UseTransformPositionForNoise>(entt::hashed_string("m_UseTransformPositionForNoise")).prop(Reflection::DISPLAY_NAME, std::string("Use transform position for noise"));

        return false;
    }

}