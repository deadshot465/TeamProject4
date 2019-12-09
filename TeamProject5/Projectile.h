#pragma once
#include "IUpdatable.h"
#include "Helper.h"

class Projectile :
	public IUpdatable
{
private:
	glm::vec2 mPosition = {};
	glm::vec2 mDestination = {};
	glm::vec2 mSpeed = {};

public:
	Projectile(float x, float y, float destX, float destY, float speed);
	~Projectile();

	bool CheckBound(int viewportWidth, int viewportHeight) const noexcept;

	// IUpdatable ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Update(int viewportWidth, int viewportHeight) override;
	virtual void Render() override;
};

