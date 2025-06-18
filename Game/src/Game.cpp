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
#include "ECS/Components/TerrainComponents.h"

#include "ECS/Systems/TerrainSystems.h"

#include "World/World.h"

#include "Utility/Profiler.h"

ENTRY_APP(Game)

namespace LAG
{
	struct InputComponent
	{
		bool jump = false;
		bool shoot = false;

		float leftStickX = 0.0f;
		float leftStickY = 0.0f;
		float rightStickX = 0.0f;
		float rightStickY = 0.0f;
	};

	struct PhysicsComponent
	{
		float mass = 1.0f;
		float drag = 0.0f;
		bool useGravity = true;

		float velocityX = 0.0f;
		float velocityY = 0.0f;
		float velocityZ = 0.0f;

		float forceX = 0.0f;
		float forceY = 0.0f;
		float forceZ = 0.0f;
	};

	struct AudioComponent
	{
		bool playing = false;
		std::string audioPath;
		bool loop = false;
		float volume = 1.f;
		float attenuation = 1.f;
	};

	struct EnemyComponent
	{
		bool alive;
		std::string name;

		float health;
		float anger;
		float strength;
	};
}

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

	LAG::GetRenderer()->SetSkyboxCubemap(LAG::HashedString("res/Assets/Cubemaps/Sky_2k"));
	LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/Helmet/DamagedHelmet.gltf"));
	LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/Cube/Cube.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/Avocado/Avocado.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/BoomBox/BoomBox.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/ChessGame/ABeautifulGame.gltf"));

	//Load an entity with a model and a transform
	LAG::Entity ent1 = LAG::GetEngine().GetScene()->AddEntity("Helmet");
	ent1.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, 0.f, 0.f));
	ent1.AddComponent<LAG::TransformComponent>()->SetScale (glm::vec3(1.f));
	//ent1.AddComponent<LAG::ModelComponent>("res/Assets/Models/ChessGame/ABeautifulGame.gltf");
	ent1.AddComponent<LAG::ModelComponent>("res/Assets/Models/Helmet/DamagedHelmet.gltf");
	ent1.AddComponent<LAG::SinWaveComponent>(6.f, 1.f);

	LAG::Entity ent2 = LAG::GetEngine().GetScene()->AddEntity("Light1");
	ent2.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, 0.f, -5.f));
	auto ent2Light = ent2.AddComponent<LAG::LightComponent>();
	ent2Light->lightAttenuation = 5.f;
	ent2Light->lightIntensity = 2.f;

	LAG::Entity ent3 = LAG::GetEngine().GetScene()->AddEntity("Light2");
	auto ent3Light = ent3.AddComponent<LAG::LightComponent>();
	ent3Light->lightAttenuation = 0.1f;
	ent3Light->lightIntensity = 2.f;
	ent3Light->lightColor = glm::vec3(0.f, 1.f, 0.f);
	ent3.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(-5.f, -2.f, -7.f));

	LAG::Entity ent4 = LAG::GetEngine().GetScene()->AddEntity("Light3");
	ent4.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(5.f, 40.f, -7.f));
	auto ent4Light = ent4.AddComponent<LAG::LightComponent>();
	ent4Light->lightAttenuation = 0.0001f;
	ent4Light->lightIntensity = 1.f;
	ent4Light->lightColor = glm::vec3(1.f, 1.f, 1.f);

	LAG::Entity ent5 = LAG::GetEngine().GetScene()->AddEntity("Camera");
	ent5.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, -2.f, 0.f));
	ent5.GetComponent<LAG::TransformComponent>()->SetRotation(glm::vec3(1.57f, 0.f, 0.f));
	ent5.AddComponent<LAG::CameraComponent>()->movementSpeed = 50.f;

	//m_World = new World(12, true);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	const int TOTAL_DIMENSION = 500;
	srand(3);

	LAG::Scene* scene = LAG::GetScene();
	for (int y = 0; y < TOTAL_DIMENSION; y++)
	{

		int enemyIndex = rand() % TOTAL_DIMENSION + 1;
		int inputIndex = rand() % TOTAL_DIMENSION + 1;
		const int withAudio = static_cast<int>(TOTAL_DIMENSION * 0.3f);
		const int withPhysics = static_cast<int>(TOTAL_DIMENSION * 0.5f);

		for (int x = 0; x < TOTAL_DIMENSION; x++)
		{
			LAG::Entity entity = scene->AddEntity();
			entity.AddComponent<LAG::TransformComponent>();
			//entity.AddComponent<LAG::ModelComponent>("res/Assets/Models/Cube/Cube.gltf");

			if (x > withAudio)
				entity.AddComponent<LAG::AudioComponent>();
			if (x > withPhysics)
				entity.AddComponent<LAG::PhysicsComponent>();

			if (enemyIndex == x)
				entity.AddComponent<LAG::EnemyComponent>();
			if (inputIndex == x)
				entity.AddComponent<LAG::InputComponent>();
		}
	}
}

void Game::Shutdown()
{
}

void Game::Update()
{
	LAG_PROFILE();

	//m_World->Update();

	LAG::Scene* scene = LAG::GetEngine().GetScene();
	scene->RunSystem<LAG::TransformComponent, LAG::ModelComponent>([](LAG::EntityID id, LAG::TransformComponent* transform, LAG::ModelComponent* model) 
		{
			const glm::vec3& currPos = transform->GetPosition();
			transform->SetPosition(glm::vec3(currPos.x, currPos.y + 0.01f, currPos.z));
		}
	);

	scene->RunSystem<LAG::TransformComponent, LAG::ModelComponent, LAG::PhysicsComponent, LAG::EnemyComponent>(
		[](
			LAG::EntityID entity,
			LAG::TransformComponent* transform, LAG::ModelComponent* model,
			LAG::PhysicsComponent* physics, LAG::EnemyComponent* enemy)
		{
			enemy->strength += 0.1f;
		});

	scene->RunSystem<LAG::TransformComponent, LAG::ModelComponent, LAG::AudioComponent, LAG::InputComponent>(
		[](
			LAG::EntityID entity,
			LAG::TransformComponent* transform, LAG::ModelComponent* model,
			LAG::AudioComponent* audio, LAG::InputComponent* input)
		{
			input->leftStickX += 0.1f;
		});
}
