#include "Game.h"
#include "Core/EntryPoint.h"

#include "Core/Input/Input.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"

#include "Platform/RenderBackend.h"
#include "Platform/Resources/Model.h"

#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/ModelComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/SurfaceComponent.h"

#include "ECS/Systems/TerrainSystems.h"

#include "Utility/Profiler.h"

#include "World/World.h"

ENTRY_APP(Game)

Game::Game()
{
}

Game::~Game()
{
}

void Game::Initialize()
{
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_W, LAG::StringHash64("UpButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_S, LAG::StringHash64("DownButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_LMB, LAG::StringHash64("LMB"));

	LAG::GetRenderer()->SetSkyboxCubemap(LAG::StringHash64("res/Assets/Cubemaps/Sky_2k"));
	LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::StringHash64("res/Assets/Models/Helmet/DamagedHelmet.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::StringHash64("res/Assets/Models/Avocado/Avocado.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::StringHash64("res/Assets/Models/BoomBox/BoomBox.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::StringHash64("res/Assets/Models/ChessGame/ABeautifulGame.gltf"));

	LAG::Scene* sc = LAG::GetScene();
	
	LAG::EntityID ent1 = sc->AddEntity("Helmet");
	{
		LAG::TransformComponent* transformComp = sc->AddComponent<LAG::TransformComponent>(ent1);
		transformComp->SetPosition(glm::vec3(0.f, 0.f, 0.f));
		transformComp->SetScale(glm::vec3(1.f));

		sc->AddComponent<LAG::ModelComponent>(ent1, "res/Assets/Models/Helmet/DamagedHelmet.gltf");
		sc->AddComponent<LAG::SinWaveComponent>(ent1, 6.f, 1.f);
	}

	LAG::EntityID ent2 = sc->AddEntity("Light1");
	{
		sc->AddComponent<LAG::TransformComponent>(ent2)->SetPosition(glm::vec3(0.f, 0.f, -5.f));
		LAG::LightComponent* lightComp = sc->AddComponent<LAG::LightComponent>(ent2);
		lightComp->lightAttenuation = 5.f;
		lightComp->lightIntensity = 2.f;
	}

	LAG::EntityID ent3 = sc->AddEntity("Light2");
	{
		sc->AddComponent<LAG::TransformComponent>(ent3)->SetPosition(glm::vec3(-5.f, -2.f, -7.f));
		LAG::LightComponent* lightComp = sc->AddComponent<LAG::LightComponent>(ent3);
		lightComp->lightAttenuation = 0.1f;
		lightComp->lightIntensity = 2.f;
		lightComp->lightColor = glm::vec3(0.f, 1.f, 0.f);
	}

	LAG::EntityID ent4 = sc->AddEntity("Light3");
	{
		sc->AddComponent<LAG::TransformComponent>(ent4)->SetPosition(glm::vec3(5.f, 40.f, -7.f));
		LAG::LightComponent* lightComp = sc->AddComponent<LAG::LightComponent>(ent4);
		lightComp->lightAttenuation = 0.0001f;
		lightComp->lightIntensity = 1.f;
		lightComp->lightColor = glm::vec3(1.f, 1.f, 1.f);
	}

	LAG::EntityID ent5 = sc->AddEntity("Camera");
	{
		LAG::TransformComponent* transformComp = sc->AddComponent<LAG::TransformComponent>(ent5);
		transformComp->SetPosition(glm::vec3(0.f, -2.f, 0.f));
		transformComp->SetRotation(glm::vec3(1.57f, 0.f, 0.f));

		sc->AddComponent<LAG::CameraComponent>(ent5)->movementSpeed = 50.f;
	}

	sc->AddEntity("Empty Entity");

	m_World = new World(12, true);
}

void Game::Shutdown()
{
}

void Game::Update()
{
	LAG_PROFILE();

	m_World->Update();

	LAG::GetScene()->ForEach<LAG::SinWaveComponent, LAG::TransformComponent>([](LAG::EntityID entity, LAG::SinWaveComponent* component, LAG::TransformComponent* transform)
		{
			component->time += LAG::GetEngine().GetDeltaTime();
			component->sinValue = component->amplitude * sinf(component->frequency * component->time);

			transform->SetPosition(transform->GetPosition() + glm::vec3(0.f, component->sinValue, 0.f));
		});
}
