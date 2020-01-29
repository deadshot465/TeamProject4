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

	mItemTextures[0] = LoadTexture("./sprite/UI.png");
	mItemTextures[1] = LoadTexture("./sprite/UI.png");
	mItemRectangles[0] = { 360, 480, 128, 128 };
	mItemRectangles[1] = { 600, 480, 128, 128 };
	mItemRectangles[2] = { 840, 480, 128, 128 };

	mInitialized = true;
}

void GameScene::Update(float deltaTime)
{
	if (!mInitialized) return;
	
	// Handle item menu.
	if (mIsPaused)
	{
		if (IsKeyPressed(KEY_ONE))
		{
			std::cout << "Select Shield.\n";
			mIsPaused = false;
		}
		else if (IsKeyPressed(KEY_TWO))
		{
			std::cout << "Select HP.\n";
			mIsPaused = false;
		}
		else if (IsKeyPressed(KEY_THREE))
		{
			std::cout << "Select Time.\n";
			mIsPaused = false;
		}

		return;
	}

	++mCounter;
	if (mCounter >= 60)
	{
		mCounter = 0;
		--mTimer;
	}

	GenerateEnemies(deltaTime, mSpeedFactor, mEnemyAttack, mThreshold);

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
			mScore += 100;
		}
		mShieldEntity->Children.clear();
	}
}

void GameScene::Render()
{
	if (!mInitialized) return;

	BeginDrawing();

	MapChipManager::DrawMapChips("StageFloor");
	MapChipManager::DrawMapChips("StageSide");
	MapChipManager::DrawMapChips("StageShadow");

	SceneBase::Render();

	// Game Pause
	if (mIsPaused)
	{
		Color color = { 0, 0, 0, 128 };
		DrawRectangle(0, 0, 1280, 960, color);
		DrawText("PAUSE", 560, 400, 72, WHITE);

		DrawTexturePro(mItemTextures[0], { 192, 0, 64, 64 }, mItemRectangles[0], { 0, 0 }, 0.0f, WHITE);
		DrawTexturePro(mItemTextures[1], { 0, 0, 64, 64 }, mItemRectangles[1], { 0, 0 }, 0.0f, WHITE);
		DrawText("TIME", mItemRectangles[2].x, mItemRectangles[2].y + 32, 56, GOLD);
	}

	DrawText(FormatText("SCORE: %d", mScore), 750, 24, 48, SKYBLUE);
	DrawText(FormatText("TIME REMAINING: %d", mTimer), 750, 72, 48, SKYBLUE);

	EndDrawing();
}

void GameScene::GenerateEnemies(float deltaTime, float speedFactor, int attack, int threshold)
{
	static float elapsed_time = 0.0f;
	static std::uniform_real_distribution<float> time_rng(0.5f, 2.0f);
	static float until_next = 0.0f;
	static bool timer_started = false;
	static std::uniform_real_distribution<float> x_rng(-500.0f, 1280.0f + 500.0f);
	static std::uniform_real_distribution<float> y_rng(-500.0f, 960.0f + 500.0f);
	std::uniform_real_distribution<float> speed_rng(5.0f * speedFactor, 10.0f * speedFactor);

	if (mEnemyCounter >= threshold)
	{
		mEnemyCounter = 0;
		elapsed_time = 0.0f;
		timer_started = false;
		mIsPaused = true;
		++mEnemyAttack;
		mSpeedFactor *= 1.2f;
		mThreshold += 5;
		return;
	}

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
			float x_value = x_rng(RANDOM_ENGINE);
			float y_value = y_rng(RANDOM_ENGINE);
			if (y_value > 0.0f && y_value < 960.0f)
			{
				while (x_value > 0.0f && x_value < 1280.0f)
					x_value = x_rng(RANDOM_ENGINE);
			}
			auto enemy = mEnemyEntities.emplace_back(this->AddEntity(GetRandomString(10)));
			enemy->Position = glm::vec2(x_value, y_value);
			auto controller = std::make_unique<EnemyController>(speed_rng(RANDOM_ENGINE), attack);
			enemy->AddComponent(controller);

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
			++mEnemyCounter;
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
