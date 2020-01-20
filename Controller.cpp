#include "Controller.h"
#include <cassert>

Controller::Controller(float speed)
	: Speed(speed)
{
}

Controller::~Controller()
{
}

void Controller::Initialize()
{
}

void Controller::Update(float deltaTime)
{
	assert(mEntity);

	auto adjusted_speed = Speed / std::sqrtf(2.0f);

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
}

void Controller::Render()
{
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
	mPlayerPosition = mEntity->CurrentScene->GetEntity("player-entity")->Position;

	if (glm::distance(mEntity->Position, mPlayerPosition) > 50)
	{
		Velocity = glm::normalize(mEntity->Position - mPlayerPosition) * Speed;
	}

	mEntity->Position += Velocity;
}

void EnemyController::Render()
{
}
