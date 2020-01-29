#include "SceneBase.h"
#include "Animator.h"
#include "Colliders.h"
#include "Controller.h"

SceneBase::SceneBase() : Scene()
{
}

SceneBase::SceneBase(std::string_view name) : Scene(name)
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Initialize()
{
	SetupPlayer();
	SetupShield();

	Scene::Initialize();
}

void SceneBase::Update(float deltaTime)
{
	Scene::Update(deltaTime);
}

void SceneBase::Render()
{
	Scene::Render();
}

void SceneBase::SetupPlayer()
{
	mPlayerEntity = this->AddEntity("player-entity");
	mPlayerEntity->Position = glm::vec2(512, 384);

	auto collider = std::make_unique<CircleCollider>("player-collider");
	mPlayerEntity->AddComponent(collider);
	auto controller = std::make_unique<PlayerController>(7.5f);
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

void SceneBase::SetupShield()
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
