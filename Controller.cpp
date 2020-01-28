#include "Controller.h"
#include <iostream>
#include <cassert>
#include "ConfigurationManager.h"
#include "Colliders.h"
#include "CollisionSystem.h"
#include "MapChipManager.h"

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
	animator->Play(NORMAL_RIGHT);

	auto adjusted_speed = Speed / std::sqrtf(2.0f);

	HandleShield();
	if (mAttackFlag) return;

	if (IsKeyDown(KEY_LEFT))
	{
		animator->Play(RUN_LEFT);
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
		{
			mEntity->Position.x -= adjusted_speed;
			mEntity->Position.y += IsKeyDown(KEY_UP) ? -adjusted_speed : adjusted_speed;
		}
		else
			mEntity->Position.x -= Speed;
	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		animator->Play(RUN_RIGHT);
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
		{
			mEntity->Position.x += adjusted_speed;
			mEntity->Position.y += IsKeyDown(KEY_UP) ? -adjusted_speed : adjusted_speed;
		}
		else
			mEntity->Position.x += Speed;
	}
	else if (IsKeyDown(KEY_UP))
	{
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
		{
			mEntity->Position.y -= adjusted_speed;
			mEntity->Position.x += IsKeyDown(KEY_LEFT) ? -adjusted_speed : adjusted_speed;
			if (IsKeyDown(KEY_LEFT))
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
	else if (IsKeyDown(KEY_DOWN))
	{
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
		{
			mEntity->Position.y += adjusted_speed;
			mEntity->Position.x += IsKeyDown(KEY_LEFT) ? -adjusted_speed : adjusted_speed;
			if (IsKeyDown(KEY_LEFT))
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

	ClampToScreenSize();
}

void PlayerController::Render()
{
	
}

void PlayerController::HandleShield()
{
	static int counter = 0;
	constexpr float duration = 10.0f;
	static constexpr float offset = 300.0f;
	static auto dest_offset = glm::vec2();	
	
	if (mAttackFlag)
	{
		++counter;
		auto collider = mEntity->GetComponent<CircleCollider>();
		assert(collider);
		auto res = CollisionSystem::CheckWallCollision(collider, MapChipManager::GetAllColliders("TitleSide"));
		if (res)
		{
			mAttackFlag = false;
			counter = 0;
			mEntity->CurrentScene->GetEntity("shield-entity")->GetComponent<ShieldController>()->Enabled = true;
			return;
		}
		mEntity->Position += dest_offset / duration;
		ClampToScreenSize();
	}

	if (IsKeyDown(KEY_Z) && !mAttackFlag)
	{
		mAttackFlag = true;
		dest_offset = glm::normalize(ToGlmVector2(GetMousePosition()) - mEntity->Position) * offset;
		mEntity->CurrentScene->GetEntity("shield-entity")->GetComponent<ShieldController>()->Enabled = false;
	}

	if (counter >= duration)
	{
		mAttackFlag = false;
		counter = 0;
		mEntity->CurrentScene->GetEntity("shield-entity")->GetComponent<ShieldController>()->Enabled = true;
	}
}

void PlayerController::ClampToScreenSize()
{
	static constexpr int frame_width = 128;
	static constexpr int frame_height = 128;
	static constexpr float x_margin = frame_width / 4.0f;
	static constexpr float y_margin = frame_height / 4.0f;
	static constexpr float chip_size = 64.0f;
	static const auto SCREEN_WIDTH = ConfigurationManager::GetWidth();
	static const auto SCREEN_HEIGHT = ConfigurationManager::GetHeight();

	if ((mEntity->Position.x - x_margin) < chip_size)
		mEntity->Position.x = x_margin + chip_size;
	if ((mEntity->Position.x + x_margin) > SCREEN_WIDTH - chip_size)
		mEntity->Position.x = SCREEN_WIDTH - x_margin - chip_size;
	
	if ((mEntity->Position.y - y_margin) < chip_size)
		mEntity->Position.y = y_margin + chip_size;
	if ((mEntity->Position.y + (y_margin * 2.0f)) > SCREEN_HEIGHT - (chip_size * 2))
		mEntity->Position.y = SCREEN_HEIGHT - (y_margin * 2.0f) - (chip_size * 2);

}

EnemyController::EnemyController(float speed)
	: Speed(speed), mShouldDestroy(false)
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
	auto collider = mEntity->GetComponent<CircleCollider>();
	
	// If the enemy has a parent entity, it means that it's already attached to the shield.
	// We don't have to manually update the speed anymore.
	if (mEntity->Parent)
	{
		return;
	}

	// If the collider is disabled, set the parent entity to the shield.
	if (collider && !collider->Enabled)
	{
		mEntity->Parent = mEntity->CurrentScene->GetEntity("shield-entity");
		mEntity->RelativePosition = mEntity->Position - mEntity->Parent->Position;
		return;
	}

	// If the speed is still being updated, follow the player until it's close to the player.
	if (!mSpeedUpdateStopped)
	{
		mPlayerPosition = mEntity->CurrentScene->GetEntity("player-entity")->Position;
		auto distance = glm::distance(mEntity->Position, mPlayerPosition);

		if (distance > 50.0f)
		{
			Velocity = glm::normalize(mPlayerPosition - mEntity->Position) * Speed;
		}
		else
		{
			mSpeedUpdateStopped = true;
		}
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
