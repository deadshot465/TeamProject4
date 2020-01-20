#include "GameScene.h"
#include "CollisionSystem.h"
#include "ConfigurationManager.h"
#include "Controller.h"
#include "Helper.h"
#include "Primitives.h"

GameScene::GameScene() : Scene()
{
	mPlayerEntity = this->AddEntity("player-entity");
	mPlayerEntity->Position = glm::vec2(512, 384);
	
	auto circle = std::make_unique<Primitives::Circle>("player-circle", 50.0f);
	circle->FillColor = GOLD;
	mPlayerEntity->AddComponent(circle);
	auto collider = std::make_unique<CircleCollider>("player-collider");
	mPlayerEntity->AddComponent(collider);
	auto controller = std::make_unique<Controller>(10.0f);
	mPlayerEntity->AddComponent(controller);
}

GameScene::GameScene(std::string_view name)
	: Scene(name)
{
	
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	Scene::Initialize();

	mInitialized = true;
}

void GameScene::Update(float deltaTime)
{
	GenerateEnemies(deltaTime);

	Scene::Update(deltaTime);
}

void GameScene::Render()
{
	BeginDrawing();

	DrawText("FUCK OFF!!!", 190, 200, 20, LIGHTGRAY);
	DrawText("FUCK OFF!!!", 380, 300, 30, LIGHTGRAY);
	DrawText("FUCK OFF!!!", 570, 400, 40, LIGHTGRAY);
	DrawText("FUCK OFF!!!", 760, 500, 50, LIGHTGRAY);

	Scene::Render();

	EndDrawing();
}

void GameScene::GenerateEnemies(float deltaTime)
{
	static float elapsed_time = 0.0f;
	static std::uniform_real_distribution<float> time_rng(0.5f, 2.0f);
	static std::uniform_real_distribution<float> speed_rng(5.0f, 10.0f);
	static std::uniform_int_distribution<> color_rng(0, 3);
	static std::uniform_int_distribution<> bool_rng(0, 1);
	static float until_next = 0.0f;
	static bool timer_started = false;
	static constexpr float x_bounds[] = { -500.0f, 1024.0f + 500.0f };
	static constexpr float y_bounds[] = { -500.0f, 768.0f + 500.0f };

	elapsed_time += deltaTime;
	
	if (!timer_started)
	{
		until_next = time_rng(RANDOM_ENGINE);
		timer_started = true;
	}
	else
	{
		if (elapsed_time > until_next)
		{
			auto enemy = mEnemyEntities.emplace_back(this->AddEntity(GetRandomString(10)));
			enemy->Position = glm::vec2(x_bounds[bool_rng(RANDOM_ENGINE)],
				y_bounds[bool_rng(RANDOM_ENGINE)]);
			auto controller = std::make_unique<EnemyController>(speed_rng(RANDOM_ENGINE));
			enemy->AddComponent(controller);

			Color color = ENEMY_COLORS[color_rng(RANDOM_ENGINE)];
			auto circle = std::make_unique<Primitives::Circle>(GetRandomString(10), 25.0f, color);
			enemy->AddComponent(circle);

			timer_started = false;
			elapsed_time = 0.0f;
		}
	}
}
