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
	UpdateHpUI();
	Scene::Update(deltaTime);
}

void SceneBase::Render()
{
	Scene::Render();
	UpdateShieldUI();
}

void SceneBase::SetupUIEntities()
{
	// HP UI
	mHpUIEntities[0] = this->AddEntity("hp-entity-1");
	mHpUIEntities[0]->Position = glm::vec2(48, 48);
	mHpUIEntities[1] = this->AddEntity("hp-entity-2");
	mHpUIEntities[1]->Position = glm::vec2(128, 48);
	mHpUIEntities[2] = this->AddEntity("hp-entity-3");
	mHpUIEntities[2]->Position = glm::vec2(208, 48);

	std::unique_ptr<Animator> animators[3];
	for (auto& animator : animators)
	{
		animator = std::make_unique<Animator>("./sprite/UI.png", 5, 1);
		animator->SetAnimation(0, "full-health", { 0 });
		animator->SetAnimation(0, "half-health", { 1 });
		animator->SetAnimation(0, "no-health", { 2 });
		animator->Play(0, "full-health");
	}

	mHpUIEntities[0]->AddComponent(animators[0]);
	mHpUIEntities[1]->AddComponent(animators[1]);
	mHpUIEntities[2]->AddComponent(animators[2]);

	// Shield UI
	mShieldUIEntities[0] = this->AddEntity("shield-ui-1");
	mShieldUIEntities[0]->Position = glm::vec2(288, 48);
	mShieldUIEntities[1] = this->AddEntity("shield-ui-2");
	mShieldUIEntities[1]->Position = glm::vec2(368, 48);

	std::unique_ptr<Animator> _animators[2];
	for (auto& animator : _animators)
	{
		animator = std::make_unique<Animator>("./sprite/UI.png", 5, 1);
		animator->SetAnimation(0, "full-shield", { 3 });
		animator->Play(0, "full-shield");
	}

	mShieldUIEntities[0]->AddComponent(_animators[0]);
	mShieldUIEntities[1]->AddComponent(_animators[1]);

	mShieldMask = LoadTexture("./sprite/UI.png");
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
}

void SceneBase::UpdateHpUI()
{
	// HP
	switch (PLAYER_CONTROLLER->Hp)
	{
	case 5:
		mHpUIEntities[2]->GetComponent<Animator>()->Play(0, "half-health");
		break;
	case 4:
		mHpUIEntities[2]->GetComponent<Animator>()->Play(0, "no-health");
		break;
	case 3:
		mHpUIEntities[1]->GetComponent<Animator>()->Play(0, "half-health");
		break;
	case 2:
		mHpUIEntities[1]->GetComponent<Animator>()->Play(0, "no-health");
		break;
	case 1:
		mHpUIEntities[0]->GetComponent<Animator>()->Play(0, "half-health");
		break;
	case 0:
		mHpUIEntities[0]->GetComponent<Animator>()->Play(0, "no-health");
		break;
	default:
		mHpUIEntities[2]->GetComponent<Animator>()->Play(0, "full-health");
		mHpUIEntities[1]->GetComponent<Animator>()->Play(0, "full-health");
		mHpUIEntities[0]->GetComponent<Animator>()->Play(0, "full-health");
		break;
	}
}

void SceneBase::UpdateShieldUI()
{
	static std::vector<unsigned char> alphas = std::vector<unsigned char>(mShieldUIEntities.size());

	// Shield
	for (auto i = 0; i < mShieldUIEntities.size(); ++i)
	{
		if (PLAYER_CONTROLLER->ShieldCooldowns[i] == false)
			continue;
		Vector2 pos = {};
		pos.x = mShieldUIEntities[i]->Position.x - 32;
		pos.y = mShieldUIEntities[i]->Position.y - 32;
		DrawTextureRec(mShieldMask, { 256, 0, 64, 64 }, pos, WHITE);
	}
}
