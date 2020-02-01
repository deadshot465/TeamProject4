#include "Controller.h"
#include <iostream>
#include <type_traits>
#include <cassert>
#include "ConfigurationManager.h"
#include "Colliders.h"
#include "CollisionSystem.h"
#include "MapChipManager.h"
#include "SFX.h"

class TitleScene;
class GameScene;

PlayerController::PlayerController(float speed)
	: Speed(speed)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Initialize()
{
}

void PlayerController::Update(float deltaTime)
{
	assert(mEntity);
	const auto& animator = PLAYER_ANIMATOR;
	static int animation_timer = 0;
	auto adjusted_speed = Speed / std::sqrtf(2.0f);

	if (IsInvincible)
	{
		++mCounter;
		if (mCounter >= 90)
		{
			mCounter = 0;
			IsInvincible = false;
		}
	}

	HandleShield();
	if (mAttackFlag) return;

	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
	{
		if (animation_timer <= 0)
		{
			animator->Play(RUN_LEFT);
			animation_timer = 30;
		}
		
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S) || IsKeyDown(KEY_W))
		{
			mEntity->Position.x -= adjusted_speed;
			mEntity->Position.y += IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) ? -adjusted_speed : adjusted_speed;
		}
		else
			mEntity->Position.x -= Speed;
	}
	else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
	{
		if (animation_timer <= 0)
		{
			animator->Play(RUN_RIGHT);
			animation_timer = 30;
		}

		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
		{
			mEntity->Position.x += adjusted_speed;
			mEntity->Position.y += IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) ? -adjusted_speed : adjusted_speed;
		}
		else
			mEntity->Position.x += Speed;
	}
	else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
	{
		if (animation_timer <= 0)
		{
			if (mEntity->Position.x < GetMousePosition().x) animator->Play(RUN_RIGHT);
			else animator->Play(RUN_LEFT);
			animation_timer = 30;
		}

		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
		{
			mEntity->Position.y -= adjusted_speed;
			mEntity->Position.x += IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) ? -adjusted_speed : adjusted_speed;
			if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
				animator->Play(RUN_LEFT);
			else
				animator->Play(RUN_RIGHT);
		}
		else
		{
			mEntity->Position.y -= Speed;
			animator->Play(RUN_RIGHT);
		}
	}
	else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
	{
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
		{
			mEntity->Position.y += adjusted_speed;
			mEntity->Position.x += IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) ? -adjusted_speed : adjusted_speed;
			if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
				animator->Play(RUN_LEFT);
			else
				animator->Play(RUN_RIGHT);
		}
		else
		{
			mEntity->Position.y += Speed;
			animator->Play(RUN_RIGHT);
		}
	}

	if (animation_timer <= 0)
	{
		if (mEntity->Position.x < GetMousePosition().x) animator->Play(NORMAL_RIGHT);
		else animator->Play(NORMAL_LEFT);
		animation_timer = 10;
	}

	--animation_timer;

	ClampToScreenSize();
}

void PlayerController::Render()
{
	
}

void PlayerController::HandleShield()
{
	static int counter = 0;
	constexpr float duration = 15.0f;
	static constexpr float offset = 1000.0f;
	static auto dest_offset = glm::vec2();
	
	std::string get_scene_collider_name = [&]() {
		if (mEntity->CurrentScene->GetName() == "title-scene") return "TitleSide";
		if (mEntity->CurrentScene->GetName() == "game-scene") return "StageSide";
	}();
	
	if (mAttackFlag)
	{
		++counter;
		auto collider = mEntity->GetComponent<CircleCollider>();
		assert(collider);
		mEntity->Position += dest_offset / duration;
		if (static_cast<float>(counter) >= duration)
		{
			auto res = CollisionSystem::CheckWallCollision(collider, MapChipManager::GetAllColliders(get_scene_collider_name));
			if (res)
			{
				SFX::PlaySfx("Explosion");
				mAttackFlag = false;
				counter = 0;
				mEntity->CurrentScene->GetEntity("shield-entity")->GetComponent<ShieldController>()->Enabled = true;
				return;
			}
		}
		ClampToScreenSize();
	}

	if (IsKeyPressed(KEY_Z) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !mAttackFlag)
	{
		auto iter = ShieldCooldowns.begin();
		while (iter != ShieldCooldowns.end())
		{
			if (*iter == true)
			{
				++iter;
				continue;
			}

			SFX::PlaySfx("Dash");
			mAttackFlag = true;
			dest_offset = glm::normalize(ToGlmVector2(GetMousePosition()) - mEntity->Position) * offset;
			mEntity->CurrentScene->GetEntity("shield-entity")->GetComponent<ShieldController>()->Enabled = false;

			auto&& temp = *iter;
			temp = true;
			++mCoolingdownShieldCount;
			break;
		}
	}

	if (counter >= duration)
	{
		mAttackFlag = false;
		counter = 0;
		mEntity->CurrentScene->GetEntity("shield-entity")->GetComponent<ShieldController>()->Enabled = true;
	}

	if (mCoolingdownShieldCount > 0)
		++mCooldownTimer;

	if (mCoolingdownShieldCount > 0 && mCooldownTimer >= 90)
	{
		mCooldownTimer = 0;
		--mCoolingdownShieldCount;
		auto iter = ShieldCooldowns.begin();
		while (iter != ShieldCooldowns.end())
		{
			if (*iter == false)
			{ 
				++iter;
				continue; 
			}
			auto&& _temp = *iter;
			_temp = false;
			break;
		}
	}
}

void PlayerController::ClampToScreenSize()
{
	static constexpr int frame_width = 128;
	static constexpr int frame_height = 128;
	static constexpr float x_margin = frame_width / 4.0f;
	static constexpr float y_margin = frame_height / 4.0f;
	static constexpr float chip_size = 64.0f;
	static const auto SCREEN_WIDTH = 1152;
	static const auto SCREEN_HEIGHT = 896;

	if ((mEntity->Position.x - x_margin) < (chip_size * 3))
		mEntity->Position.x = x_margin + (chip_size * 3);
	if ((mEntity->Position.x + x_margin) > SCREEN_WIDTH - chip_size)
		mEntity->Position.x = SCREEN_WIDTH - x_margin - chip_size;
	
	if ((mEntity->Position.y - y_margin) < (chip_size * 3))
		mEntity->Position.y = y_margin + (chip_size * 3);
	if ((mEntity->Position.y + (y_margin * 2.0f)) > SCREEN_HEIGHT - (chip_size * 2))
		mEntity->Position.y = SCREEN_HEIGHT - (y_margin * 2.0f) - (chip_size * 2);

}

EnemyController::EnemyController(float speed, int attack)
	: Speed(speed), mShouldDestroy(false), Attack(attack)
{
}

EnemyController::~EnemyController()
{
}

void EnemyController::Initialize()
{
}

void EnemyController::Update(float deltaTime)
{
	assert(mEntity);
	ENEMY_ANIMATOR->Play(NORMAL_RIGHT);
	auto player_entity = mEntity->CurrentScene->GetEntity("player-entity");

	switch (CurrentState)
	{
	case EnemyController::EnemyState::Initializing:
		CurrentState = EnemyState::Moving;
	case EnemyController::EnemyState::Moving:
	{
		if (mSpeedUpdateStopped) break;
		mPlayerPosition = player_entity->Position;
		auto distance = glm::distance(mEntity->Position, mPlayerPosition);

		// Set animation.
		if (mEntity->Position.x > mPlayerPosition.x) ENEMY_ANIMATOR->Play(RUN_LEFT);
		else ENEMY_ANIMATOR->Play(RUN_RIGHT);

		if (distance > 100.0f)
		{
			Velocity = glm::normalize(mPlayerPosition - mEntity->Position) * Speed;
		}
		else
		{
			mSpeedUpdateStopped = true;
		}

		auto player_collider = player_entity->GetComponent<CircleCollider>();
		auto collider = mEntity->GetComponent<CircleCollider>();
		auto player_controller = player_entity->GetComponent<PlayerController>();
		if (CollisionSystem::CheckCircleCollision(player_collider, collider) &&
			!player_controller->IsAttacking() && !player_controller->IsInvincible)
		{
			player_controller->Hp -= 1;
			player_controller->IsInvincible = true;
		}
		break;
	}
	case EnemyController::EnemyState::Attached:
	{
		auto collider = mEntity->GetComponent<CircleCollider>();
		if (mEntity->Parent) return;
		if (!mEntity->Parent && collider)
		{
			mEntity->Parent = mEntity->CurrentScene->GetEntity("shield-entity");
			mEntity->RelativePosition = mEntity->Position - mEntity->Parent->Position;
			mEntity->Parent->Children.emplace(mEntity);
			return;
		}
		break;
	}
	case EnemyController::EnemyState::Finalizing:
		break;
	default:
		break;
	}

	// If the enemy flies out of the bounds, set the destroy flag to true.
	if (CheckBound(mEntity->Position))
	{
		mShouldDestroy = true;
	}

	mEntity->Position += Velocity;
}

void EnemyController::Render()
{
}

ShieldController::ShieldController(int offset)
	: Offset(offset)
{

}

void ShieldController::Initialize()
{
}

void ShieldController::Update(float deltaTime)
{
	assert(mEntity);
	assert(mEntity->Parent);
	
	// If it's not enabled, it means that the player is dashing.
	if (!Enabled) return;

	// Constantly update the relative position (offset).
	auto mouse_pos = ToGlmVector2(GetMousePosition());
	auto direction = glm::normalize(mouse_pos - mEntity->Parent->Position);
	mEntity->RelativePosition = direction * static_cast<float>(Offset);
	
	auto x_axis = glm::vec2(1500.0f, mEntity->Parent->Position.y) - mEntity->Parent->Position;
	auto x_axis_normalized = glm::normalize(x_axis);
	auto dot = glm::dot(direction, x_axis_normalized);
	auto length_product = glm::length(direction) * glm::length(x_axis_normalized);
	auto angle = glm::degrees(glm::acos(dot / length_product));

	if (angle != 0)
	{
		auto animator = mEntity->GetComponent<Animator>();
		if (animator)
		{
			animator->SetAngle(angle);
		}
	}
}

void ShieldController::Render()
{
}
