#include "Controller.h"
#include <iostream>
#include <cassert>
#include "Animator.h"
#include "ConfigurationManager.h"
#include "Colliders.h"

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

	// TODO: The moving speed is not correct. Assign to: Lee.
	auto adjusted_speed = Speed / std::sqrtf(2.0f);

	HandleShield();
	if (mAttackFlag) return;

	if (IsKeyDown(KEY_LEFT))
	{
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
		{
			mEntity->Position.x -= adjusted_speed;
			mEntity->Position.y += IsKeyDown(KEY_UP) ? -adjusted_speed : adjusted_speed;
		}
		else
			mEntity->Position.x -= Speed;
	}

	if (IsKeyDown(KEY_RIGHT))
	{
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
		{
			mEntity->Position.x += adjusted_speed;
			mEntity->Position.y += IsKeyDown(KEY_UP) ? -adjusted_speed : adjusted_speed;
		}
		else
			mEntity->Position.x += Speed;
	}

	if (IsKeyDown(KEY_UP))
	{
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
		{
			mEntity->Position.y -= adjusted_speed;
			mEntity->Position.x += IsKeyDown(KEY_LEFT) ? -adjusted_speed : adjusted_speed;
		}
		else
			mEntity->Position.y -= Speed;
	}

	if (IsKeyDown(KEY_DOWN))
	{
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
		{
			mEntity->Position.y += adjusted_speed;
			mEntity->Position.x += IsKeyDown(KEY_LEFT) ? -adjusted_speed : adjusted_speed;
		}
		else
			mEntity->Position.y += Speed;
	}

	ClampToScreenSize();
}

void PlayerController::Render()
{
	
}

void PlayerController::HandleShield()
{
	static int counter = 0;
	constexpr float duration = 15.0f;
	constexpr float offset = 200.0f;
	static auto dest_offset = glm::vec2();	
	
	if (mAttackFlag)
	{
		++counter;
		mEntity->Position += dest_offset / duration;
	}

	if (IsKeyDown(KEY_Z))
	{
		mAttackFlag = true;
		dest_offset = glm::normalize(ToGlmVector2(GetMousePosition()) - mEntity->Position) * offset;
	}

	if (counter >= duration)
	{
		mAttackFlag = false;
		counter = 0;
	}
}

void PlayerController::ClampToScreenSize()
{
	constexpr int frame_width = 128;
	constexpr int frame_height = 128;
	constexpr float x_margin = frame_width / 4.0f;
	constexpr float y_margin = frame_height / 4.0f;
	const auto SCREEN_WIDTH = ConfigurationManager::GetWidth();
	const auto SCREEN_HEIGHT = ConfigurationManager::GetHeight();

	if ((mEntity->Position.x - x_margin) < 0)
		mEntity->Position.x = x_margin;
	if ((mEntity->Position.x + x_margin) > SCREEN_WIDTH)
		mEntity->Position.x = SCREEN_WIDTH - x_margin;
	if ((mEntity->Position.y - y_margin) < 0)
		mEntity->Position.y = y_margin;
	if ((mEntity->Position.y + (y_margin * 2.0f)) > SCREEN_HEIGHT)
		mEntity->Position.y = SCREEN_HEIGHT - (y_margin * 2.0f);

}

EnemyController::EnemyController(float speed)
	: Speed(speed)
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
	if (mEntity->Parent)
	{
		return;
	}
	if (collider && !collider->Enabled)
	{
		mEntity->Parent = mEntity->CurrentScene->GetEntity("shield-entity");
		mEntity->RelativePosition = mEntity->Position - mEntity->Parent->Position;
		return;
	}

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

	if (CheckBound(mEntity->Position))
	{
		mShouldDestroy = true;
	}

	mEntity->Position += Velocity;
}

void EnemyController::Render()
{
}

bool EnemyController::ShouldDestroy() const noexcept
{
	return mShouldDestroy;
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
