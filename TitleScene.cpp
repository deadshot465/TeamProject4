#include "TitleScene.h"
#include <cassert>
#include "Animator.h"
#include "Colliders.h"
#include "CollisionSystem.h"
#include "Controller.h"
#include "Helper.h"
#include "MapChipManager.h"

TitleScene::TitleScene() : SceneBase()
{
}

TitleScene::TitleScene(std::string_view name)
	: SceneBase(name)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	SceneBase::Initialize();

	MapChipManager::LoadMapChip("./map/title_side.csv", "./sprite/map.png", "TitleSide", 11, 5);
	MapChipManager::LoadMapChip("./map/title_floor.csv", "./sprite/map.png", "TitleFloor", 11, 5);
	MapChipManager::LoadMapChip("./map/title_shadow.csv", "./sprite/map.png", "TitleShadow", 11, 5);

	mInitialized = true;
}

void TitleScene::Update(float deltaTime)
{
	if (!mInitialized) return;
	SceneBase::Update(deltaTime);

	// Shield collision
	auto shield_collider = mShieldEntity->GetComponent<CircleCollider>();
	auto start_activated = CollisionSystem::StartActivated(shield_collider);
	auto quit_activated = CollisionSystem::QuitActivated(shield_collider);

	if (start_activated)
	{
		assert(SceneChangeHandler);
		OnSceneChanged(int(Scenes::GameScene));
	}
	else if (quit_activated)
	{
		assert(SceneChangeHandler);
		OnSceneChanged(int(Scenes::End));
	}
}

void TitleScene::Render()
{
	if (!mInitialized) return;

	BeginDrawing();

	MapChipManager::DrawMapChips("TitleFloor");
	MapChipManager::DrawMapChips("TitleSide");
	MapChipManager::DrawMapChips("TitleShadow");

	SceneBase::Render();

	EndDrawing();
}
