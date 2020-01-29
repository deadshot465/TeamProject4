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
#include "SFX.h"

GameScene::GameScene() : SceneBase()
{
	
}

GameScene::GameScene(std::string_view name)
	: SceneBase(name)
{
	
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	SceneBase::Initialize();

	MapChipManager::LoadMapChip("./map/stage_side.csv", "./sprite/map.png", "StageSide", 11, 5);
	MapChipManager::LoadMapChip("./map/stage_floor.csv", "./sprite/map.png", "StageFloor", 11, 5);
	MapChipManager::LoadMapChip("./map/stage_shadow.csv", "./sprite/map.png", "StageShadow", 11, 5);

	mInitialized = true;
}

void GameScene::Update(float deltaTime)
{
	if (!mInitialized) return;

	GenerateEnemies(deltaTime);

	SceneBase::Update(deltaTime);

	// Remove enemies that are out of range.
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

	// Shield collision
	auto shield_collider = mShieldEntity->GetComponent<CircleCollider>();
	CircleCollider object;
	auto res = CollisionSystem::CheckShieldCollision(shield_collider, GetAllEnemyColliders(), &object);

	res = CollisionSystem::CheckWallCollision(shield_collider, MapChipManager::GetAllColliders("StageSide"));

	if (res)
	{
		for (const auto& child : mShieldEntity->Children)
		{
			mEnemyEntities.erase(std::remove_if(mEnemyEntities.begin(), mEnemyEntities.end(), [&child](const Entity* entity) {
				return entity == child;
				}), mEnemyEntities.end());

			this->RemoveEntity(child->GetName());
		}
		mShieldEntity->Children.clear();
	}

	/*iter = mEnemyEntities.begin();
	while (iter != mEnemyEntities.end())
	{
		if ((*iter)->GetComponent<EnemyController>()->CurrentState !=
			EnemyController::EnemyState::Attached)
		{
			++iter;
			continue;
		}

		auto res = CollisionSystem::CheckWallCollision((*iter)
			->GetComponent<CircleCollider>(), MapChipManager::GetAllColliders("TitleSide"));

		if (res)
		{
			this->RemoveEntity((*iter)->GetName());
			iter = mEnemyEntities.erase(iter);
		}
		else
			++iter;
	}*/
}

void GameScene::Render()
{
	if (!mInitialized) return;

	BeginDrawing();

	MapChipManager::DrawMapChips("StageFloor");
	MapChipManager::DrawMapChips("StageSide");
	MapChipManager::DrawMapChips("StageShadow");

	SceneBase::Render();

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

			//Color color = ENEMY_COLORS[color_rng(RANDOM_ENGINE)];
			//auto circle = std::make_unique<Primitives::Circle>(GetRandomString(10), 25.0f, color);
			//enemy->AddComponent(circle);

			std::string_view file_names[] = {
				"./sprite/monster_1_normal_right.png",
				"./sprite/monster_1_run_right.png",
				"./sprite/monster_1_normal_left.png",
				"./sprite/monster_1_run_left.png"
			};

			int x_count[] = { 4, 4, 4, 4 };
			int y_count[] = { 1, 1, 1, 1 };

			auto animator = std::make_unique<Animator>(file_names, x_count, y_count);
			animator->SetAnimation(0, "normal-right", { 0, 1, 2, 3 });
			animator->SetAnimation(1, "run-right", { 0, 1, 2, 3 });
			animator->SetAnimation(2, "normal-left", { 0, 1, 2, 3 });
			animator->SetAnimation(3, "run-left", { 0, 1, 2, 3 });
			animator->Play(1, "run-right");
			enemy->AddComponent(animator);

			auto collider = std::make_unique<CircleCollider>(GetRandomString(10));
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
