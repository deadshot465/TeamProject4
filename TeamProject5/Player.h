#pragma once
#include <list>
#include <raylib.h>
#include "IUpdatable.h"
#include "Helper.h"
#include "Projectile.h"

class Player : public IUpdatable
{
private:
	Position mPosition = Position();

	std::list<Projectile> mProjectiles;

	void HandleInput();
	void HandleAttack();
	void UpdateProjectiles(int viewportWidth, int viewportHeight);

public:
	Player(float x, float y);
	~Player();

	// IUpdatable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Update(int viewportWidth, int viewportHeight) override;

	virtual void Render() override;

};

