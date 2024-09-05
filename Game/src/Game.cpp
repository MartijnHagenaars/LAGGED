#include "Game.h"
#include "Core/EntryPoint.h"

#include "Core/Input/Input.h"

#include "Core/Engine.h"
#include "Platform/Renderer.h"

#include "Core/Memory/ResourcePtr.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Model.h"

#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/ModelComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/TerrainComponents.h"

#include "ECS/Systems/TerrainSystems.h"



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
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_W, LAG::HashedString("UpButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_S, LAG::HashedString("DownButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_LMB, LAG::HashedString("LMB"));

	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/Helmet/DamagedHelmet.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/Avocado/Avocado.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/BoomBox/BoomBox.gltf"));
	LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/ChessGame/ABeautifulGame.gltf"));

	//Load an entity with a model and a transform
	LAG::Entity ent1 = LAG::GetEngine().GetScene()->AddEntity("Helmet");
	ent1.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, 5.f, 0.f));
	ent1.AddComponent<LAG::TransformComponent>()->SetScale (glm::vec3(6.f));
	ent1.AddComponent<LAG::ModelComponent>("res/Assets/Models/ChessGame/ABeautifulGame.gltf");
	//ent1.AddComponent<LAG::ModelComponent>("res/Assets/Models/Helmet/DamagedHelmet.gltf");
	ent1.AddComponent<LAG::SinWaveComponent>(6.f, 1.f);

	LAG::Entity ent2 = LAG::GetEngine().GetScene()->AddEntity("Light1");
	ent2.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, 0.f, -5.f));
	auto ent2Light = ent2.AddComponent<LAG::LightComponent>();
	ent2Light->lightAttenuation = 5.f;
	ent2Light->lightIntensity = 2.f;

	LAG::Entity ent3 = LAG::GetEngine().GetScene()->AddEntity("Light2");
	ent3.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(-5.f, -2.f, -7.f));
	auto ent3Light = ent3.AddComponent<LAG::LightComponent>();
	ent3Light->lightAttenuation = 0.1f;
	ent3Light->lightIntensity = 2.f;
	ent3Light->lightColor = glm::vec3(0.f, 1.f, 0.f);

	LAG::Entity ent4 = LAG::GetEngine().GetScene()->AddEntity("Light3");
	ent4.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(5.f, 40.f, -7.f));
	auto ent4Light = ent4.AddComponent<LAG::LightComponent>();
	ent4Light->lightAttenuation = 0.0001f;
	ent4Light->lightIntensity = 1.f;
	ent4Light->lightColor = glm::vec3(1.f, 1.f, 1.f);

	LAG::Entity ent5 = LAG::GetEngine().GetScene()->AddEntity("Camera");
	ent5.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, -10.f, 0.f));
	ent5.GetComponent<LAG::TransformComponent>()->SetRotation(glm::vec3(1.57f, 0.f, 0.f));
	ent5.AddComponent<LAG::CameraComponent>()->movementSpeed = 80.f;

	LAG::Entity ent6 = LAG::GetEngine().GetScene()->AddEntity("Surface");
	ent6.AddComponent<LAG::TransformComponent>()->SetScale(glm::vec3(1.f));
	//ent6.AddComponent<LAG::SurfaceComponent>();
	ent6.AddComponent<LAG::ProceduralSurfaceComponent>();
	LAG::SurfaceSystems::GenerateNoiseSurface(&ent6);

	m_World = new World(8, true);
}

void Game::Shutdown()
{
}

void Game::Update()
{
	m_World->Update();

	LAG::GetRenderer()->DrawLine(glm::vec3(0.f), glm::vec3(10.f), glm::vec3(1.f, 0.f, 0.f));
}
