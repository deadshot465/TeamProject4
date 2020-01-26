#pragma once
#include "ECS.h"

class PlayerController :
	public Component
{
public:
	PlayerController(float speed);
	~PlayerController();

	// Component ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	float Speed = 0.0f;

private:
	void HandleShield();
	void ClampToScreenSize();

	bool mAttackFlag = false;
};

class EnemyController
	: public Component
{
public:
	EnemyController(float speed);
	~EnemyController();

	bool ShouldDestroy() const noexcept;

	float Speed = 0.0f;
	glm::vec2 Velocity = {};
	
protected:
	// Component ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

private:
	glm::vec2 mPlayerPosition = {};
	bool mSpeedUpdateStopped = false;
	bool mShouldDestroy = false;
};

class ShieldController
	: public Component
{
public:
	ShieldController(int offset);
	~ShieldController() = default;

	int Offset = 0;

protected:
	// Component ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
};
