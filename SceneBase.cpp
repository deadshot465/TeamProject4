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
	SetupUIEntities();
	SetupPlayer();
	SetupShield();

	Scene::Initialize();
}

void SceneBase::Update(float deltaTime)
{
	UpdateUI();
	Scene::Update(deltaTime);
}

void SceneBase::Render()
{
	Scene::Render();
}

void SceneBase::SetupUIEntities()
{
	mHpEntities[0] = this->AddEntity("hp-entity-1");
	mHpEntities[0]->Position = glm::vec2(48, 48);
	mHpEntities[1] = this->AddEntity("hp-entity-2");
	mHpEntities[1]->Position = glm::vec2(128, 48);
	mHpEntities[2] = this->AddEntity("hp-entity-3");
	mHpEntities[2]->Position = glm::vec2(208, 48);

	std::unique_ptr<Animator> animators[3];
	for (auto& animator : animators)
	{
		animator = std::make_unique<Animator>("./sprite/UI.png", 5, 1);
		animator->SetAnimation(0, "full-health", { 0 });
		animator->SetAnimation(0, "half-health", { 1 });
		animator->SetAnimation(0, "no-health", { 2 });
		animator->Play(0, "full-health");
	}

	mHpEntities[0]->AddComponent(animators[0]);
	mHpEntities[1]->AddComponent(animators[1]);
	mHpEntities[2]->AddComponent(animators[2]);
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

void SceneBase::UpdateUI()
{
	switch (PLAYER_CONTROLLER->Hp)
	{
	case 5:
		mHpEntities[2]->GetComponent<Animator>()->Play(0, "half-health");
		break;
	case 4:
		mHpEntities[2]->GetComponent<Animator>()->Play(0, "no-health");
		break;
	case 3:
		mHpEntities[1]->GetComponent<Animator>()->Play(0, "half-health");
		break;
	case 2:
		mHpEntities[1]->GetComponent<Animator>()->Play(0, "no-health");
		break;
	case 1:
		mHpEntities[0]->GetComponent<Animator>()->Play(0, "half-health");
		break;
	case 0:
		mHpEntities[0]->GetComponent<Animator>()->Play(0, "no-health");
		break;
	default:
		mHpEntities[2]->GetComponent<Animator>()->Play(0, "full-health");
		mHpEntities[1]->GetComponent<Animator>()->Play(0, "full-health");
		mHpEntities[0]->GetComponent<Animator>()->Play(0, "full-health");
		break;
	}
}
