#include "GameScene.h"
#include <algorithm>
#include <iostream>
#include "Animator.h"
#include "CollisionSystem.h"
#include "ConfigurationManager.h"
#include "Controller.h"
#include "Helper.h"
#include "MapChipManager.h"
#include "Primitives.h"

GameScene::GameScene() : Scene()
{
	
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
	SetupPlayer();
	SetupShield();

	Scene::Initialize();

	MapChipManager::LoadMapChip("./map/title_side.csv", "./sprite/map.png", "TitleSide", 11, 5);
	MapChipManager::LoadMapChip("./map/title_floor.csv", "./sprite/map.png", "TitleFloor", 11, 5);
	MapChipManager::LoadMapChip("./map/title_shadow.csv", "./sprite/map.png", "TitleShadow", 11, 5);

	mInitialized = true;
}

void GameScene::Update(float deltaTime)
{
	GenerateEnemies(deltaTime);

	Scene::Update(deltaTime);

	auto iter = mEnemyEntities.begin();
	while (iter != mEnemyEntities.end())
	{
		if ((*iter)->GetComponent<EnemyController>()->ShouldDestroy())
		{
			this->RemoveEntity((*iter)->GetName());
			iter = mEnemyEntities.erase(iter);
		}
		else
			++iter;
	}

	auto shield_collider = mShieldEntity->GetComponent<CircleCollider>();
	CircleCollider object;
	auto res = CollisionSystem::CheckShieldCollision(shield_collider, GetAllEnemyColliders(), &object);
	if (res) std::cout << "Collision Detected.\n";
}

void GameScene::Render()
{
	BeginDrawing();

	MapChipManager::DrawMapChips("TitleFloor");
	MapChipManager::DrawMapChips("TitleSide");
	MapChipManager::DrawMapChips("TitleShadow");

	Scene::Render();

	EndDrawing();
}

void GameScene::SetupPlayer()
{
	mPlayerEntity = this->AddEntity("player-entity");
	mPlayerEntity->Position = glm::vec2(512, 384);

	auto collider = std::make_unique<CircleCollider>("player-collider");
	mPlayerEntity->AddComponent(collider);
	auto controller = std::make_unique<PlayerController>(10.0f);
	mPlayerEntity->AddComponent(controller);

	std::string_view file_names[] = {
		"./sprite/player_normal_right.png",
		"./sprite/player_run_right.png",
		"./sprite/player_normal_left.png",
		"./sprite/player_run_left.png"
	};

	int x_count[] = { 4, 7, 4, 7 };
	int y_count[] = { 1, 1, 1, 1 };

	auto animator = std::make_unique<Animator>(file_names, x_count, y_count);
	animator->SetAnimation(0, "normal-right", { 0, 1, 2, 3 });
	animator->SetAnimation(1, "run-right", { 0, 1, 2, 3, 4, 5, 6 });
	animator->SetAnimation(2, "normal-left", { 0, 1, 2, 3 });
	animator->SetAnimation(3, "run-left", { 0, 1, 2, 3, 4, 5, 6 });
	animator->Play(0, "normal-right");
	mPlayerEntity->AddComponent(animator);
}

void GameScene::SetupShield()
{
	// Shield
	mShieldEntity = this->AddEntity("shield-entity");
	mShieldEntity->Parent = mPlayerEntity;
	auto shield_controller = std::make_unique<ShieldController>(50);
	mShieldEntity->AddComponent(shield_controller);

	auto shield_animator = std::make_unique<Animator>("./sprite/shield.png", 2, 1);
	shield_animator->SetAnimation(0, "none", { 0 });
	shield_animator->Play(0, "none");
	mShieldEntity->AddComponent(shield_animator);

	auto shield_collider = std::make_unique<CircleCollider>("shield-collider", 32.0f);
	mShieldEntity->AddComponent(shield_collider);

	/*auto shield_circle = std::make_unique<Primitives::Circle>(GetRandomString(10), 32.0f);
	mShieldEntity->AddComponent(shield_circle);*/
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

			auto collider = std::make_unique<CircleCollider>(GetRandomString(10), 25.0f);
			enemy->AddComponent(collider);

			timer_started = false;
			elapsed_time = 0.0f;
		}
	}
}

std::vector<CircleCollider*> GameScene::GetAllEnemyColliders() const noexcept
{
	auto colliders = std::vector<CircleCollider*>(mEnemyEntities.size());
	std::transform(mEnemyEntities.cbegin(), mEnemyEntities.cend(), colliders.begin(),
		[&](Entity* entity) {
			return entity->GetComponent<CircleCollider>();
		});
	return colliders;
}
