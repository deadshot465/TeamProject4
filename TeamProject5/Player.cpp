#include "Player.h"

void Player::HandleInput()
{

	if (IsKeyDown(KEY_RIGHT)) mPosition.Absolute.x += 2.0f;
	if (IsKeyDown(KEY_LEFT)) mPosition.Absolute.x -= 2.0f;
	if (IsKeyDown(KEY_UP)) mPosition.Absolute.y -= 2.0f;
	if (IsKeyDown(KEY_DOWN)) mPosition.Absolute.y += 2.0f;

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) HandleAttack();
}

void Player::HandleAttack()
{
	auto pos = GetMousePosition();

	mProjectiles.emplace_front(Projectile(mPosition.Absolute.x,
		mPosition.Absolute.y, pos.x, pos.y, 8.0f));
}

void Player::UpdateProjectiles(int viewportWidth, int viewportHeight)
{
	auto iter = mProjectiles.begin();

	while (iter != mProjectiles.end()) {
		iter->Update(viewportWidth, viewportHeight);

		if (iter->CheckBound(viewportWidth, viewportHeight)) {
			iter = mProjectiles.erase(iter);
		}
		else {
			++iter;
		}
	}
}

Player::Player(float x, float y) : mPosition(glm::vec2(x, y))
{
	mProjectiles = std::list<Projectile>();
}

Player::~Player()
{
	
}

void Player::Update(int viewportWidth, int viewportHeight)
{
	HandleInput();
	UpdateProjectiles(viewportWidth, viewportHeight);
}

void Player::Render()
{
	DrawCircleV(ToRayLibVector2(mPosition), 50.0f, GOLD);

	for (auto& proj : mProjectiles)
		proj.Render();
}
