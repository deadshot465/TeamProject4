#include "GameScene.h"
#include <algorithm>
#include <iostream>
#include "Animator.h"
#include "CollisionSystem.h"
#include "ConfigurationManager.h"
#include "Controller.h"
#include "Helper.h"
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
	mPlayerEntity = this->AddEntity("player-entity");
	mPlayerEntity->Position = glm::vec2(512, 384);

	auto collider = std::make_unique<CircleCollider>("player-collider");
	mPlayerEntity->AddComponent(collider);
	auto controller = std::make_unique<Controller>(10.0f);
	mPlayerEntity->AddComponent(controller);
	
	std::string_view file_names[] = {
		"./sprite/player_normal.png",
		"./sprite/player_run_right.png"
	};

	int x_count[] = { 4, 7 };
	int y_count[] = { 1, 1 };

	auto animator = std::make_unique<Animator>(file_names, x_count, y_count);
	animator->SetAnimation(0, "normal", { 0, 1, 2, 3 });
	animator->SetAnimation(1, "run-right", { 0, 1, 2, 3, 4, 5, 6 });
	animator->Play(1, "run-right");
	mPlayerEntity->AddComponent(animator);

	// Shield
	mShieldEntity = this->AddEntity("shield-entity");
	mShieldEntity->Parent = mPlayerEntity;
	auto shield_controller = std::make_unique<ShieldController>(65);
	mShieldEntity->AddComponent(shield_controller);
	
	auto shield_animator = std::make_unique<Animator>("./sprite/shield_right.png", 1, 1);
	shield_animator->SetAnimation(0, "none", { 0 });
	shield_animator->Play(0, "none");
	mShieldEntity->AddComponent(shield_animator);

	// TODO: Shield's collider is not in the correct position. Assign to: Chou.
	auto shield_collider = std::make_unique<CircleCollider>("shield-collider", 32);
	mShieldEntity->AddComponent(shield_collider);

	Scene::Initialize();

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
	auto res = CollisionSystem::CheckCollision(*shield_collider, GetAllEnemyColliders(), &object);
	if (res) std::cout << "Collision Detected.\n";
}

void GameScene::Render()
{
	BeginDrawing();

	/*DrawText("FUCK OFF!!!", 190, 200, 20, LIGHTGRAY);
	DrawText("FUCK OFF!!!", 380, 300, 30, LIGHTGRAY);
	DrawText("FUCK OFF!!!", 570, 400, 40, LIGHTGRAY);
	DrawText("FUCK OFF!!!", 760, 500, 50, LIGHTGRAY);*/

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

			auto collider = std::make_unique<CircleCollider>(GetRandomString(10), 25.0f);
			enemy->AddComponent(collider);

			timer_started = false;
			elapsed_time = 0.0f;
		}
	}
}

std::vector<Collider*> GameScene::GetAllEnemyColliders() const noexcept
{
	auto colliders = std::vector<Collider*>(mEnemyEntities.size());
	std::transform(mEnemyEntities.cbegin(), mEnemyEntities.cend(), colliders.begin(),
		[&](Entity* entity) {
			return entity->GetComponent<CircleCollider>();
		});
	return colliders;
}
